define(['angular'], function(angular){
    angular.module('bullseye', [])
    .directive('bullseye', [function(){
        return {
            restrict: 'E',
            template: '<div style="min-height: 100%; min-width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: center;"><div ng-transclude></div></div>',
            transclude: true,
            replace: true
        };
    }]);
});