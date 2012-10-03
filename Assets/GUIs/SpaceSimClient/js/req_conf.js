var require = {
    baseUrl: "js",
    // NOTE: If you edit this, you have to also modify the similar object in
    //    vsn/deploy/fabfile.py in function ws where it sets the paths
    paths: {
        "underscore": "external/underscore",
        "jquery": "external/jquery-1.7.2",
        "backbone": "external/backbone",
        "text": "external/text",
        "domReady": "external/domReady",
        "mediaelement": "external/mediaelement/mediaelement-and-player",
        "moment": "external/moment"
    },
    shim: {
        /* NOTE: I still don't feel like this is working properly! */
        "external/bootstrap": ["jquery"],
        // This fails when using the path name for some reason!
        "backbone": {
            deps: ["underscore", "jquery"],
            exports: "Backbone"
        },
        "external/backbone.subroute": ["backbone"],
        "external/Backbone.ModelBinder": ["backbone"]
    },
    deps: [
        "underscore",
        "jquery",
        "external/bootstrap"
    ]
};
