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
		e.append("<table>");
                e.append("<tr><td>Lit: <input id=\"lit\" type=\"checkbox\" /></td><td>Shadows: <input id=\"shadows\" type=\"checkbox\" /></td><td></td></tr>");
                e.append("<tr><td>Decal: </td><td><input type=\"text\" id=\"decal\"/></td><td><button id=\"set_decal\">Set</button></td></tr>");
                e.append("<tr><td>Normal: </td><td><input type=\"text\" id=\"normal\"/></td><td><button id=\"set_normal\">Set</button></td></tr>");
                e.append("<tr><td>Height: </td><td><input type=\"text\" id=\"height\"/></td><td><button id=\"set_height\">Set</button></td></tr>");
                e.append("<tr><td>Specular: </td><td><input type=\"text\" id=\"specular\"/></td><td><button id=\"set_specular\">Set</button></td></tr>");
		e.append("<tr><td>Save File: </td><td><input type=\"text\" id=\"output_path\"/></td><td><button id=\"save\">Save</button></td></tr>");
		e.append("</table>");
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
		$("#set_decal").button();
		$("#set_normal").button();
		$("#set_height").button();
		$("#set_specular").button();
		$("#save").button();

		$("#lit").click(function() {
                    $.get("set_lit?lit=" + $("#lit").is(":checked"), function(data){
			console.log("Object Lit Toggled");
                    });
		});
		$("#shadows").click(function() {
		    $.get("set_shadows?shadows=" + $("#shadows").is(":checked"), function(data){
			console.log("Object Shadows Toggled");
		    });
		});
		$("#set_decal").click(function() {
		    var decal_path = encodeURIComponent($("#decal").val());
		    $.get("set_decal?decal=" + decal_path, function(data){
			console.log("Object Decal Changed");
		    });
		});
		$("#set_normal").click(function() {
                    var normal_path = encodeURIComponent($("#normal").val());
		    $.get("set_normal?normal=" + normal_path, function(data){
			console.log("Object Normal Changed");
		    });
		});
                $("#set_height").click(function() {
                    var height_path = encodeURIComponent($("#height").val());
                    $.get("set_height?normal=" + height_path, function(data){
                        console.log("Object Height Changed");
                    });
                });
                $("#set_specular").click(function() {
                    var specular_path = encodeURIComponent($("#specular").val());
                    $.get("set_specular?specular=" + specular_path, function(data){
                        console.log("Object Specular Changed");
                    });
                });
		$("#save").click(function() {
		    var output_path = encodeURIComponent($("#output_path").val());
		    $.get("save_model?output_path=" + output_path, function(data){
			e.empty();
			$("#load_model_dialog").empty();
			$("#load_model_dialog").dialog("close");
		    });
		});
            } else {
                $("#load_model_dialog").html("Model Failed to Load!");
            }
        });
    });
});
