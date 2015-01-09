define(['./module-declaration', './color-utils', './color-picker', './right-click'], function(colorPicker, colorUtils, rightClick){
    colorPicker
    .directive('colorSwatch', ['colorUtils', function(colorUtils){
        return {
            restrict: 'A',
            templateUrl: 'modules/color-picker/swatch.html',
            transclude: true,
            replace: true,
            scope: {
                rgb: '='
            },
            controller: ['$scope', function($scope){
                $scope.isColorPickerOpen = false;
                $scope.openColorPicker = function() {
                    $scope.isColorPickerOpen = true;
                };

                $scope.$on('color-picker: picked-new-color', function(ev, newColor){
                    ev.stopPropagation();

                    var newColorHexInteger = parseInt(newColor, 16);
                    $scope.rgb = colorUtils.hexToRgb(newColorHexInteger);

                    $scope.isColorPickerOpen = false;
                });

                $scope.$on('color-picker: canceled-color-picking', function(ev){
                    ev.stopPropagation();

                    $scope.isColorPickerOpen = false;
                });

                $scope.$watch('rgb', function(rgb){
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