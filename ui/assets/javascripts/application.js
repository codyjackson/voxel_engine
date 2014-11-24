require.config({
    'paths': {
        'jquery': 'vendor/jquery/jquery',
        'angular': 'vendor/angular/angular'
    },
    'shim': {
        'jquery': {
            'exports': '$'
        },
        'angular': {
            'deps': ['jquery'],
            'exports': 'angular'
        }
    },
});

require(['angular', 'directives/bullseye'], function(angular){
    angular.module('ui', ['bullseye']);
    angular.element(document).ready(function(){
        angular.bootstrap(document, ['ui']);
    });

    console.log(window.test);

    window.onkeydown = function(ev){
        switch(ev.keyCode) {
            case 87:
                api.player.startMovingForward().then(function(){
                    console.log('forward');
                });
                break;
            case 83:
                api.player.startMovingBackward();
                break;
            case 68:
                api.player.rotateYaw(0.5).then(function(){}, function(error){
                    console.log(error);
                });
                break;
            case 65:
                api.player.rotateYaw(-0.5).then(function(){}, function(error){
                    console.log(error);
                });
                break;
        }
    };

    window.onkeyup = function(ev){
        switch(ev.keyCode) {
            case 87:
            case 83:
                api.player.stopMovingForwardOrBackward();
                break;
            case 68:
            case 65:
                api.player.stopMovingLeftOrRight();
                break;
        }
    };

    document.documentElement.onmousedown = api.input.mouse.lockMovement;
    document.documentElement.onmouseup = api.input.mouse.unlockMovement;
});
