define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");

    var Get = function(path, callback) {
        $.get(path, callback);
    };

    var Post = function(path, data, callback) {
        var url = path;
        var attr_name;
        var attr_value;
        var first_attr = true;
        for (attr_name in data) {
            if (data.hasOwnProperty(attr_name)) {
                if (first_attr === true) {
                    url += "?";
                    first_attr = false;
                }
                attr_value = data[attr_name];
                url += attr_name + "=" + encodeURIComponent(attr_value);
            }
        }
        $.get(url, callback);
    };

    return {
        Get: Get,
        Post: Post
    };
});
