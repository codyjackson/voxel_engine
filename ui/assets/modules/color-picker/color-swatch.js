define(['angular', './module-declaration'], function(angular, colorPicker){
    colorPicker
    .directive('colorSwatch', [function(){
        return {
            restrict: 'A',
            templateUrl: 'swatch.html',
            transclude: true,
            replace: true,
            link: function(scope, element, attrs) {
            }
        };
    }]);
});