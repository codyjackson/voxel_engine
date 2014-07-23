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
});
