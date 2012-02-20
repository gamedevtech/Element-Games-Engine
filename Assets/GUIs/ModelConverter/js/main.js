head.js("js/jquery-1.7.1.min.js", "js/jquery-ui-1.8.17.custom.min.js", "js/underscore-min.js", "js/backbone-min.js",
function() {
	var w = window;
	$("#load_model_dialog").dialog();
	$("#load_model_dialog").find("#load").click(function() {
		$.post("load_model", {'filename': $("#filename").val()}, function(data){
			console.log(data);
		});
	});
});

