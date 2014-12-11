define(['./color-picker', './color-utils'], function(colorPicker, colorUtils){

    colorPicker.directive('colorSvPicker', ['colorUtils', function(colorUtils){
        return {
            restrict: 'A',
            template: '<canvas></canvas>',
            transclude: true,
            replace: true,
            link: function(scope, element, attrs) {
                var canvasElement = $(element)[0];
                var context = canvasElement.getContext('2d');
                var rect = {
                    width: $(canvasElement).innerWidth(),
                    height: $(canvasElement).innerHeight()
                };
                context.canvas.width = rect.width;
                context.canvas.height = rect.height;
                var image = context.createImageData(rect.width, rect.height);
                var imageData = image.data;
                var numberOfColorChannels = 4;

                for(var i = 0; i < imageData.length; ++i) {
                    imageData[i] = 255;
                }

                for(var y = 0; y < rect.height; ++y) {
                    for(var x = 0; x < rect.width; ++x) {
                        var i = ((rect.width * y) + x) * numberOfColorChannels;
                        var h = 0;
                        var s = x / (rect.width - 1);
                        var v = 1 - (y / (rect.height - 1));
                        var hsv = new colorUtils.Hsv(h, s, v);
                        var rgb = hsv.toRgb();
                        imageData[i+0] = rgb.r;
                        imageData[i+1] = rgb.g;
                        imageData[i+2] = rgb.b;
                        imageData[i+3] = 255;
                    }
                }
                context.putImageData(image, 0, 0);
            },
            scope: true,
            controller: ['$scope', function($scope){
                $scope.chooseColor = function($event) {
                    console.log('hello');
                };
            }]
        };
    }]);
});