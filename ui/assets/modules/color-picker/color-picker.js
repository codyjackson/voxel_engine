define(['./module-declaration','./color-utils', './color-hue-picker', './color-sv-picker'], function(colorPickerModule, colorUtils){

    colorPickerModule.directive('colorPicker', ['colorUtils', '$parse', function(colorUtils, $parse){
        function constrain(x, low, high) {
            if(x < low) {
                return low;
            }

            if(x > high) {
                return high;
            }

            return x;
        }

        return {
            restrict: 'A',
            templateUrl: 'modules/color-picker/color-picker.html',
            transclude: true,
            replace: true,
            scope: {
                hue: '=',
                saturation: '=',
                value: '='
            },
            controller: ['$scope', function($scope){
                $scope.hue = 0;
                $scope.saturation = 0;
                $scope.value = 0;
                $scope.getColorStagingAreaBackground = function(){
                    var hsv = new colorUtils.Hsv($scope.hue, $scope.saturation, $scope.value);
                    var rgb = hsv.toRgb();
                    var rgbString = rgb.toRgbString();
                    return {'background-color': rgbString};
                };

            }]
        };
    }]);
});