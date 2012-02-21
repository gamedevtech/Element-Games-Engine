head.js("js/json2.js", "js/jquery-1.7.1.min.js", "js/jquery-ui-1.8.17.custom.min.js", "js/underscore-min.js", "js/backbone-min.js",
function() {
    var w = window;
    $("#load_model_dialog").dialog();
    $("#load_model_dialog").find("#load").click(function() {
        $.get("load_model?filename=" + $("#filename").val(), function(json_raw){
            var data = JSON.parse(json_raw.toString());
            if (data['status'] === true) {
                console.log("RD True!");
            } else {
                console.log("RD False!");
            }
        });
    });
});
