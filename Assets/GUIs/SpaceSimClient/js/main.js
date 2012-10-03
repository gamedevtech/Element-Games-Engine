define(function(require) {
    var _ = require("underscore");
    var $ = require("jquery");
    var domReady = require("domReady");
    var util = require("util");

    var main_menu = require("main_menu");
    var network_browser = require("network_browser");

    var MenuRouter = Backbone.Router.extend({
        routes: {
            "": "main",
            "join": "join",
            "options": "options",
            "quit": "quit"
        },
        initialize: function() {
            _.bindAll(this, "main");
            _.bindAll(this, "join");
            _.bindAll(this, "options");
            _.bindAll(this, "quit");
            _.bindAll(this, "connected");

            this.el = $("#content");
        },
        main: function() {
            this.el.empty();
            $("#back_button").hide();
            var mmv = new main_menu.MainMenu();
            mmv.render();
            this.el.append(mmv.el);
        },
        join: function() {
            this.el.empty();
            $("#back_button").show();
            var servers = new network_browser.Servers();
            var nb = new network_browser.LANBrowser({
                collection: servers
            });
            nb.bind("connected", this.connected);
            servers.begin_fetch();
            nb.render();
            this.el.append(nb.el);
        },
        options: function() {
            this.el.empty();
            $("#back_button").show();
            //
        },
        quit: function() {
            util.Get("quit", function(response) {
                console.log("G'Bye!");
            });
        },
        connected: function() {
            $("#all").hide();
        }
    });

    domReady(function() {
        var mr = new MenuRouter();
        Backbone.history.start();
    });
});
