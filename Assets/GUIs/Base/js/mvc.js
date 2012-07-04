define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var util = require("util");
    var Backbone = require("backbone");

    var EGModel = Backbone.Model.extend({
        initialize: function() {
            //
        }
    });

    var EGView = Backbone.View.extend({
    });

    return {
        EGModel: Model,
        EGView: View
    };
});
