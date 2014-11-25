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
    var app = angular.module('ui', ['bullseye']);
    app.controller('GlobalController', ['$rootScope', function($rootScope){
        var mouseDown = false;
        var changingOrientation = false;
        var lockedLocation = {x:0, y:0};
        $rootScope.onMouseMove = function(ev) {
            if(!mouseDown) {
                return;
            }

            if(!changingOrientation) {
                api.input.mouse.hideCursor();
            }

            changingOrientation = true;
            var dx = ev.screenX - lockedLocation.x;
            var dy = ev.screenY - lockedLocation.y;
            api.player.rotate(dx*0.5, dy*0.5);
        };

        $rootScope.onMouseDown = function(ev) {
            mouseDown = true;
            lockedLocation.x = ev.screenX 
            lockedLocation.y = ev.screenY;
            api.input.mouse.lockMovement(lockedLocation.x, lockedLocation.y);
        };

        $rootScope.onMouseUp = function(ev) {
            changingOrientation = false;
            mouseDown = false;
            api.input.mouse.unlockMovement();
            api.input.mouse.showCursor();
        };
    }]);

    angular.element(document).ready(function(){
        angular.bootstrap(document, ['ui']);
    });

    window.onkeydown = function(ev){
        switch(ev.keyCode) {
            case 87:
                api.player.startMovingForward();
                break;
            case 83:
                api.player.startMovingBackward();
                break;
            case 68:
                api.player.startMovingRight();
                break;
            case 65:
                api.player.startMovingLeft();
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
});

