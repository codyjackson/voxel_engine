define(['./module-declaration','./color-utils', './color-hue-picker', './color-sv-picker'], function(colorPickerModule, colorUtils){

    colorPickerModule.directive('colorPicker', ['colorUtils', '$parse', function(colorUtils, $parse){
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

                $scope.currentColor = '#000';
                $scope.stagingAreaBackgroundColor = {'background-color': $scope.currentColor};
                $scope.$watchGroup(['hue', 'saturation', 'value'], function(){
                    var hsv = new colorUtils.Hsv($scope.hue, $scope.saturation, $scope.value);
                    var rgb = hsv.toRgb();
                    $scope.stagingAreaBackgroundColor['background-color'] = rgb.toHexString();
                    $scope.currentColor = $scope.stagingAreaBackgroundColor['background-color'].slice(1);
                });

                $scope.updateCurrentColorFromHexInput = function(color){
                    if(color.length !== 6) {
                        return;
                    }
                    color = parseInt(color, 16);
                    var r = (color & 0xFF0000) >> 16;
                    var g = (color & 0x00FF00) >> 8;
                    var b = color & 0x0000FF;
                    var rgb = new colorUtils.Rgb(r, g, b);
                    var hsv = rgb.toHsv();
                    $scope.hue = hsv.h;
                    $scope.saturation = hsv.s;
                    $scope.value = hsv.v;
                };
            }]
        };
    }]);
});