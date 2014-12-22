define(['angular', './module-declaration', './color-picker'], function(angular, colorPicker){
    colorPicker
    .directive('colorSwatch', [function(){
        return {
            restrict: 'A',
            templateUrl: 'modules/color-picker/swatch.html',
            transclude: true,
            replace: true,
            link: function(scope, element, attrs) {
                scope.isColorPickerOpen = false;
                scope.openColorPicker = function() {
                    scope.isColorPickerOpen = true;
                };
            }
        };
    }]);
});