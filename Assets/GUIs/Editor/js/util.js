define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");

    var Get = function(path, callback) {
        $.get(path, function(json_raw){
			var data = JSON.parse(json_raw.toString());
			callback(data);
		});
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
                } else {
					url += "&";
				}
                attr_value = data[attr_name];
                url += attr_name + "=" + encodeURIComponent(attr_value);
            }
        }
		console.log(url);
        $.get(url, function(json_raw) {
			console.log("Raw: ", json_raw);
			var data = JSON.parse(json_raw.toString());
			callback(data);
		});
    };

    return {
        Get: Get,
        Post: Post
    };
});
