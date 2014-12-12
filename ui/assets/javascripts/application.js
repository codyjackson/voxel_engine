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

require(['angular', 'directives/bullseye', '../modules/color-picker/color-sv-picker', '../modules/color-picker/color-hue-picker'], function(angular){
    var app = angular.module('ui', ['bullseye', 'color-picker']);
    app.factory('$mouseNavigation', [function(){
        var isMouseAssociated = false;
        var mouseLockLocation = {x: window.innerWidth/2, y:window.innerHeight/2};
        return {
            toggleMousePlayerAssociation: function(){
                isMouseAssociated = !isMouseAssociated;
                if(isMouseAssociated) {
                    api.input.mouse.lockMovement(mouseLockLocation.x, mouseLockLocation.y);
                    api.input.mouse.hideCursor();
                } else {
                    api.input.mouse.unlockMovement();
                    api.input.mouse.showCursor();
                }
            },
            mouseMove: function(x, y) {
                if(isMouseAssociated) {
                    var dx = x - mouseLockLocation.x;
                    var dy = y - mouseLockLocation.y;

                    api.player.rotate(dx*0.5, dy*0.5);
                }
            }
        };
    }]);

    app.controller('GlobalController', ['$rootScope', '$mouseNavigation', function($rootScope, $mouseNavigation){
        $rootScope.test = { hue: 30};
        var mouseDown = false;
        var changingOrientation = false;
        var lockedLocation = {x:0, y:0};
        $rootScope.onMouseMove = function(ev) {
            $mouseNavigation.mouseMove(ev.screenX, ev.screenY);
        };
    }]);

    angular.element(document).ready(function(){
        angular.bootstrap(document, ['ui']);
    });

    var angularInjector = angular.element(document).injector();
    var mouseNavigationService = angularInjector.get('$mouseNavigation');
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
            case 341:
                mouseNavigationService.toggleMousePlayerAssociation();
                break;
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

