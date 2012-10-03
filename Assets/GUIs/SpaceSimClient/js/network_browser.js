define(function(require) {
    var $ = require("jquery");
    var _ = require("underscore");
    var Backbone = require("backbone");
    var util = require("util");

    var Server = Backbone.Model.extend();
    var Servers = Backbone.Collection.extend({
        model: Server,
        initialize: function() {
            _.bindAll(this, "begin_fetch");
            _.bindAll(this, "read_results");
        },
        begin_fetch: function() {
            var t = this;
            util.Get("lan_servers", function(data) {
                t.trigger("request_sent");
                setTimeout(function() {
                    t.read_results();
                }, 3000);
            });
        },
        read_results: function() {
            var t = this;
            util.Get("lan_results", function(data) {
                console.log("HERE");
                t.reset(data);
                t.trigger("change");
            });
        }
    });

    var LANBrowser = Backbone.View.extend({
        tagName: "div",
        className: "row centered",
        events: {
            "click .server_link": "connect"
        },
        initialize: function() {
            _.bindAll(this, "connect");
            _.bindAll(this, "render");
            _.bindAll(this, "render_servers");
            this.collection.bind("change", this.render_servers);
        },
        connect: function(event) {
            var el = $(event.currentTarget);
            var ip = el.data("id");
            var t = this;
            console.log("Connecting To: " + ip);
            util.Post("connect", {ip: ip}, function(response) {
                console.log("connected");
                t.trigger("connected");
            });
        },
        render: function() {
            var template = '<div id="lan_browser"><img src="img/ajax-loader.gif" /></div>';
            this.$el.html(template);
            return this;
        },
        render_servers: function() {
            var html = '<h3>Servers...</h3>';
            this.collection.each(function(m) {
                html += '<div><a href="javascript:;" class="server_link" data-id="' + m.id.toString() + '">' + m.id.toString() + '</a></div>';
            });
            this.$el.html(html);
            return this;
        }
    });

    return {
        Servers: Servers,
        LANBrowser: LANBrowser
    };
});
