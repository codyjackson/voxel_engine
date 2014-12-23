define(['./module-declaration','./color-utils', './color-hue-picker', './color-sv-picker', './click-net'], function(colorPickerModule, colorUtils){

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
                    var colorHexInt = parseInt(color, 16);
                    var rgb = colorUtils.hexToRgb(colorHexInt);
                    var hsv = rgb.toHsv();
                    $scope.hue = hsv.h;
                    $scope.saturation = hsv.s;
                    $scope.value = hsv.v;
                };

                var eventNamespace = 'color-picker: ';
                $scope.ok = function() {
                    $scope.$emit(eventNamespace + 'picked-new-color', $scope.currentColor);
                };

                $scope.cancel = function() {
                    $scope.$emit(eventNamespace + 'canceled-color-picking');
                };
            }]
        };
    }]);
});