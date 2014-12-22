define(['./module-declaration', './color-utils', './color-picker'], function(colorPicker, colorUtils){
    colorPicker
    .directive('colorSwatch', ['colorUtils', function(colorUtils){
        return {
            restrict: 'A',
            templateUrl: 'modules/color-picker/swatch.html',
            transclude: true,
            replace: true,
            scope: true,
            controller: ['$scope', function($scope){
                $scope.isColorPickerOpen = false;
                $scope.openColorPicker = function() {
                    $scope.isColorPickerOpen = true;
                };

                $scope.currentRgb = new colorUtils.Rgb(255, 0, 255);
                $scope.$on('color-picker: picked-new-color', function(ev, newColor){
                    ev.stopPropagation();

                    var newColorHexInteger = parseInt(newColor, 16);
                    $scope.currentRgb = colorUtils.hexToRgb(newColorHexInteger);
                    
                    $scope.isColorPickerOpen = false;
                });

                $scope.$on('color-picker: canceled-color-picking', function(ev){
                    ev.stopPropagation();

                    $scope.isColorPickerOpen = false;
                });

                $scope.$watch('currentRgb', function(rgb){
                    var hsv = rgb.toHsv();
                    $scope.hue = hsv.h;
                    $scope.saturation = hsv.s;
                    $scope.value = hsv.v;

                    $scope.currentColorBackground = {'background-color': rgb.toHexString()};
                });
            }]
        };
    }]);
});