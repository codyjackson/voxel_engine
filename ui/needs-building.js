var fs = require('fs');
var walk = require('walk');


var profiles = {
    'debug': 'debug-build',
    'release': 'release-build'
};

var profilePaths = {
    'debug': 'profiles/' + profiles.debug + '.js',
    'release': 'profiles/' + profiles.release + '.js'
};

var sources = [
    'profiles',
    'assets',
    'views',
    'routes',
    'mimosa-config.js',
    'package.json',
    'server.js'
];

function getAllModificationTimestampsRecursively(path) {
    var stat = fs.statSync(path);
    var isPath = stat.size > 0;
    if(isPath) {
        return stat.mtime;
    }

    var dates = [];
    walk.walkSync(path, {
        listeners: {
            files: function(root, stats, next) {
                dates.push(stats[0].mtime);
                next();
            }
        }
    });

    return dates;
}

function getMostRecentUpdateTimestamp(dates) {
    dates.sort();
    dates = dates.reverse();
    return dates.length > 0 ? dates[0] : null;
}

function getCompiledDir(profilePath) {
    var profile = fs.readFileSync(profilePath, "utf8");
    profile = profile.replace('exports.config', '');
    profile = profile.replace('=', '');

    var o = JSON.parse(profile);
    return o.watch.compiledDir;
}

function flatten(a) {
    return a.reduce(function(l, r){
        return l.concat(r);
    }, []);
}

function needsBuilding(target) {
    var compiledDir = getCompiledDir(profilePaths[target]);
    if(!fs.existsSync(compiledDir)) {
        return true;
    }


    function getTimeStamp(time) {
        return time.getTime();
    }
    var sourceTimestamps = flatten(sources.map(getAllModificationTimestampsRecursively)).map(getTimeStamp);
    var buildTimestamps = flatten(getAllModificationTimestampsRecursively(compiledDir)).map(getTimeStamp);

    var command = 'mimosa build -o -P ' + target + '-build';
    if(sourceTimestamps.length == 0 || buildTimestamps.length == 0) {
        return true;
    }

    var mostRecentSourceTimestamp = getMostRecentUpdateTimestamp(sourceTimestamps);
    var mostRecentBuildTimestamp = getMostRecentUpdateTimestamp(buildTimestamps);

    if (mostRecentSourceTimestamp > mostRecentBuildTimestamp) {
        return true;
    }

    return false;
}

if(needsBuilding(process.argv[2])) {
    console.log('UI Needs Building.');
    process.exit(-1);
} else {
    console.log('UI Is Already Built.');
    process.exit(0);
}