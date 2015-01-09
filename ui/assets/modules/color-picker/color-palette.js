define(['./module-declaration', './color-utils', './left-click', './color-swatch'], function(colorPickerModule, colorUtils){
    colorPickerModule.directive('colorPalette', ['$parse', 'colorUtils', function($parse, colorUtils){
        function randomChannel() {
            return Math.random()*255;
        }

        function randomColor() {
            return new colorUtils.Rgb(randomChannel(), randomChannel(),randomChannel());
        }
        return {
            restrict: 'A',
            templateUrl: 'modules/color-picker/color-palette.html',
            scope: {
                rgb: '=',
            },
            controller:['$scope', function($scope){
                $scope.colors = [
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor(),
                    randomColor()
                ];

                $scope.rgb = $scope.colors[0];
                $scope.getSelectedColor = function() {
                    return $scope.colors = []
                };

                $scope.selected = 0;
                $scope.select = function(index) {
                    $scope.selected = index;
                    $scope.rgb = $scope.colors[index];
                };

                $scope.getSelectedClass = function(index) {
                    if(index === $scope.selected) {
                        return 'selected';
                    }
                    return '';
                };
            }]
        };
    }]);
});