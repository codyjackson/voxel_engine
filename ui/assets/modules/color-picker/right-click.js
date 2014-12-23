define(['./module-declaration',], function(colorPickerModule){
    colorPickerModule.directive('rightClick', ['$parse', function($parse){
        return {
            restrict: 'A',
            link: function(scope, element, attrs) {
                var fn = null;
                scope.$watch('rightClick', function(expression){ 
                    fn = $parse(attrs['rightClick']);
                });

                element.on('contextmenu', function(ev){
                    ev.preventDefault();
                    fn(scope);
                });
            }
        };
    }]);
});