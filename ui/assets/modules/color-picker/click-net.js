define(['./module-declaration',], function(colorPickerModule){
    colorPickerModule.directive('clickNet', ['$parse', function($parse){
        return {
            restrict: 'A',
            template: '<div style="position:fixed; top: 0; right: 0; bottom: 0; left: 0" ng-click="fn()" right-click="fn()"></div>',
            link: function(scope, element, attrs) {
                scope.$watch('clickNet', function(){
                    scope.fn = function() {
                        $parse(attrs['clickNet'])(scope);
                    };
                });
            }
        };
    }]);
});