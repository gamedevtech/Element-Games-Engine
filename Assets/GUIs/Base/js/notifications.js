define(function(require) {
    "use strict";

    var $ = require("jquery");
    var _ = require("underscore");
    var Backbone = require("backbone");

    var Notification = Backbone.Model.extend({
        defaults: {
            timeout: 5, // in seconds
            message: "",
            created: "1970-01-01T00:00:00",
            level: "warn" // (yellow), "error" (red), "success" (green) or "info" (blue)
        },
        initialize: function() {
            _.bindAll(this, "sync");
            _.bindAll(this, "timed_out");
            this.created = ""; // TODO: Set this to now()
            setTimeout(this.timed_out, this.get("timeout") * 1000.0);
        },
        timed_out: function() {
            this.trigger("timeout", this.id);
        },
        sync: function(method, model, options) {
            // Do nothing
        }
    });

    var Notifications = Backbone.Collection.extend({
        initialize: function() {
            _.bindAll(this, "get_next_id");
        },
        get_next_id: function() {
            var n;
            var id = -1;
            var found = false;
            while (!found) {
                id += 1;
                n = this.get(id);
                if (_.isUndefined(n)) {
                    found = true;
                }
            }
            return id;
        }
    });

    var NotificationView = Backbone.View.extend({
        tagName: "div",
        className: "alert fade in",
        initialize: function() {
            _.bindAll(this, "render");
        },
        render: function() {
            this.$el.empty();
            var level = this.model.get("level");
            if (level !== "warn") {
                this.$el.addClass("alert-" + level);
            }
            this.$el.append('<a class="close" data-dismiss="alert" href="#">x</a>');
            this.$el.append('<p>' + this.model.get("message") + '</p>');
            this.$el.alert();
            return this;
        }
    });

    var NotificationsView = Backbone.View.extend({
        tagName: "div",
        className: "notifications",
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "render_notification");
            _.bindAll(this, "notify");
            _.bindAll(this, "handle_timeout");
            this.views = {};
        },
        render: function() {
            this.$el.empty();
            return this;
        },
        notify: function(message, level, timeout) {
            var ndata = {
                message: message
            };
            if (!_.isUndefined(level)) {
                ndata.level = level;
            }
            if (!_.isUndefined(timeout)) {
                ndata.timeout = timeout;
            }
            ndata.id = this.collection.get_next_id();
            var n = new Notification(ndata);
            n.bind("timeout", this.handle_timeout);
            this.collection.add(n);
            this.render_notification(n);
        },
        render_notification: function(n) {
            var v = new NotificationView({
                model: n
            });
            v.render();
            this.$el.prepend(v.$el);
            this.views[n.id] = v;
        },
        handle_timeout: function(id) {
            if (this.views.hasOwnProperty(id)) {
                this.views[id].$el.alert('close');
            }
            var n = this.collection.get(id);
            if (!_.isUndefined(n)) {
                this.collection.remove(n);
            }
        }
    });

    $("body").append('<div id="notifications_area">');
    var notifications_collection = new Notifications();
    var notifications_area = new NotificationsView({
        el: $("#notifications_area"),
        collection: notifications_collection
    });
    notifications_area.render();

    return notifications_area.notify;
});
