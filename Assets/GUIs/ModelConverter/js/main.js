head.js("js/json2.js", "js/jquery-1.7.1.min.js", "js/jquery-ui-1.8.17.custom.min.js", "js/underscore-min.js", "js/backbone-min.js",
function() {
    var w = window;
    $("#load_model_dialog").dialog();
    $("#load_model_dialog").find("#load").click(function() {
        var filename = encodeURIComponent($("#filename").val());
        $.get("load_model?filename=" + filename, function(json_raw){
            var data = JSON.parse(json_raw.toString());
            if (data['status'] === true) {
                var e = $("#load_model_dialog");
                e.empty();
                e.append("<div>Model Loaded...</div>");
                e.append("<div><span>Lit: </span><input id=\"lit\" type=\"checkbox\" /><span>Shadows: </span><input id=\"shadows\" type=\"checkbox\" /></div>");
                e.append("<div><span>Decal:</span><input type=\"text\" id=\"decal\"/></div>");
                e.append("<div><span>Normal:</span><input type=\"text\" id=\"normal\"/></div>");
                e.append("<div><span>Height:</span><input type=\"text\" id=\"height\"/></div>");
                e.append("<div><span>Specular:</span><input type=\"text\" id=\"specular\"/></div>");
                if (data['lit']){
                    $("#lit").attr("checked", "checked");
                }
                if (data['shadows']){
                    $("#shadows").attr("checked", "checked");
                }
                $("#decal").val(data['decal']);
                $("#normal").val(data['normal']);
                $("#height").val(data['height']);
                $("#specular").val(data['specular']);
            } else {
                $("#load_model_dialog").html("Model Failed to Load!");
            }
        });
    });
});
