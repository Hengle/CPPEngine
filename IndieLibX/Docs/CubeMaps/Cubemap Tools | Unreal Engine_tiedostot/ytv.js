/*
 * YouTube TV
 *
 * Copyright 2013, Jacob Kelley - http://jakiestfu.com/
 * Released under the MIT Licence
 * http://opensource.org/licenses/MIT
 *
 * Github:  
 * Version: 1.0.2
 */
/*jslint browser: true, undef:true, unused:true*/
/*global define, module, ender */

(function (win, doc) {
    'use strict';
    var YTV = YTV || function (id, opts) {

        var noop = function () { },
            settings = {
                width: '100%',
                async: true,
                videoid: null,
                element: null,
                user: null,
                fullscreen: false,
                accent: '#fff',
                controls: true,
                annotations: false,
                autoplay: false,
                chainVideos: true,
                browsePlaylists: false,
                wmode: 'opaque',
                events: {
                    videoReady: noop,
                    stateChange: noop
                }
            },

            cache = {},
            utils = {
                events: {
                    addEvent: function addEvent(element, eventName, func) {
                        if (element.addEventListener) {
                            return element.addEventListener(eventName, func, false);
                        } else if (element.attachEvent) {
                            return element.attachEvent("on" + eventName, func);
                        }
                    },
                    removeEvent: function addEvent(element, eventName, func) {
                        if (element.addEventListener) {
                            return element.removeEventListener(eventName, func, false);
                        } else if (element.attachEvent) {
                            return element.detachEvent("on" + eventName, func);
                        }
                    },
                    prevent: function (e) {
                        if (e.preventDefault) {
                            e.preventDefault();
                        } else {
                            e.returnValue = false;
                        }
                    }
                },
                addCSS: function (css) {
                    var head = doc.getElementsByTagName('head')[0],
                        style = doc.createElement('style');
                    style.type = 'text/css';
                    if (style.styleSheet) {
                        style.styleSheet.cssText = css;
                    } else {
                        style.appendChild(doc.createTextNode(css));
                    }
                    head.appendChild(style);
                },
                addCommas: function (str) {
                    var x = str.split('.'),
                        x1 = x[0],
                        x2 = x.length > 1 ? '.' + x[1] : '',
                        rgx = /(\d+)(\d{3})/;
                    while (rgx.test(x1)) {
                        x1 = x1.replace(rgx, '$1' + ',' + '$2');
                    }
                    return x1 + x2;
                },
                parentUntil: function (el, attr) {
                    while (el.parentNode) {
                        if (el.getAttribute && el.getAttribute(attr)) {
                            return el;
                        }
                        el = el.parentNode;
                    }
                    return null;
                },
                ajax: {
                    get: function (url, fn) {
                        var handle;
                        if (win.XMLHttpRequest) {
                            handle = new XMLHttpRequest();
                        } else {
                            handle = new ActiveXObject("Microsoft.XMLHTTP");
                        }
                        handle.onreadystatechange = function () {
                            if (handle.readyState === 4 && handle.status === 200) {
                                fn.call(this, JSON.parse(handle.responseText));
                            }
                        };
                        handle.open("GET", url, true);
                        handle.send();
                    }
                },
                endpoints: {
                    base: 'https://gdata.youtube.com/',
                    userInfo: function () {
                        return utils.endpoints.base + 'feeds/api/users/' + settings.user + '?v=2&alt=json';
                    },
                    userVids: function () {
                        return utils.endpoints.base + 'feeds/api/users/' + settings.user + '/uploads/?v=2&alt=json&format=5&max-results=50';
                    },
                    userPlaylists: function () {
                        return utils.endpoints.base + 'feeds/api/users/' + settings.user + '/playlists/?v=2&alt=json&format=5&max-results=50';
                    },
                    playlistVids: function () {
                        return utils.endpoints.base + 'feeds/api/playlists/' + (settings.playlist) + '?v=2&alt=json&format=5&max-results=50';
                    }
                },
                deepExtend: function (destination, source) {
                    var property;
                    for (property in source) {
                        if (source[property] && source[property].constructor && source[property].constructor === Object) {
                            destination[property] = destination[property] || {};
                            utils.deepExtend(destination[property], source[property]);
                        } else {
                            destination[property] = source[property];
                        }
                    }
                    return destination;
                }
            },
            prepare = {
                youtube: function (fn) {
                    var tag = doc.createElement('script');
                    tag.src = "https://www.youtube.com/iframe_api";
                    var firstScriptTag = doc.getElementsByTagName('script')[0];
                    firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);
                    win.onYouTubeIframeAPIReady = fn;
                },
                build: function () {
                    settings.element.className = "ytv-canvas";
                    if (settings.fullscreen) {
                        settings.element.className += " ytv-full";
                    }
                    utils.addCSS('.ytv-list .ytv-active a{border-left-color: ' + (settings.accent) + ';}');
                },
                playlists: function (res) {
                    if (res && res.feed) {
                        var list = '<div class="ytv-playlists"><ul>',
                            playlists = res.feed.entry,
                            i;
                        for (i = 0; i < playlists.length; i++) {
                            var data = {
                                title: playlists[i].title.$t,
                                plid: playlists[i].yt$playlistId.$t,
                                thumb: playlists[i].media$group.media$thumbnail[1].url
                            };
                            list += '<a href="#" data-ytv-playlist="' + (data.plid) + '">';
                            list += '<div class="ytv-thumb"><div class="ytv-thumb-stroke"></div><img src="' + (data.thumb) + '"></div>';
                            list += '<span>' + (data.title) + '</span>';
                            list += '</a>';
                        }
                        list += '</ul></div>';

                        var lh = doc.getElementsByClassName('ytv-list-header')[0],
                            headerLink = lh.children[0];
                        headerLink.href = "#";
                        headerLink.target = "";
                        headerLink.setAttribute('data-ytv-playlist-toggle', 'true');
                        doc.getElementsByClassName('ytv-list-header')[0].innerHTML += list;
                        lh.className += ' ytv-has-playlists';
                    }
                },
                compileList: function (data) {
                    if (data && data.feed) {
                        utils.ajax.get(utils.endpoints.userInfo(), function (userInfo) {
                            var list = '',
                                user = {
                                    title: userInfo.entry.title.$t,
                                    url: 'http://youtube.com/user/' + userInfo.entry.yt$username.$t,
                                    thumb: userInfo.entry.media$thumbnail.url,
                                    summary: userInfo.entry.summary.$t,
                                    subscribers: userInfo.entry.yt$statistics.subscriberCount,
                                    views: userInfo.entry.yt$statistics.totalUploadViews
                                },
                                videos = data.feed.entry,
                                first = true,
                                i;
                            if (settings.playlist) {
                                user.title = (data.feed.media$group.media$title.$t);
                                user.description = data.feed.media$group.media$description.$t;
                                user.categories = new Array();
                                if (user.description.indexOf("Categories:") != -1) {
                                    var cats = user.description.substring(user.description.indexOf("Categories:") + 11).split(',');
                                    for (i = 0; i < cats.length; i++) {
                                        user.categories.push(cats[i].trim());
                                    }
                                }
                            }
                            var listheader = '<div class="ytv-list-header">';
                            listheader += '<a href="' + (user.url) + '" target="_blank">';
                            listheader += '<img src="' + (user.thumb) + '">';
                            listheader += '</a>';
                            listheader += '</div>';
                            $('#user_info').html(listheader);

                            var playerInfo = document.createElement('DIV');
                            playerInfo.id = 'videoinfo';
                            playerInfo.innerHTML = '<div id="user-controls" style="position;relative;overflow:hidden;background:#e2e3e4;"></div><div class="ytv-video-description" id="videodescription"></div><div id="video-list-container" style="margin:1em 0em 0em;"><div class="link_list_title" id="video-list-title"></div><div id="video-list" style="position;relative;"></div></div>';
                            settings.element.parentNode.insertBefore(playerInfo, settings.element.nextSibling);

                            //$('#user-controls').html('<div class="ytv-divider" title="Click to Toggle Video List"></div>');
                            $('#user-controls').html('<div class="ytv-button chain" onclick="toggleChaining();" title="Click to Toggle Video Chaining"></div><div class="ytv-button next"></div><div class="ytv-button prev"></div>');

                            var chain = readCookie('video_chain');
                            if (chain != null) {
                                if (chain == 'true') {
                                    $('.ytv-button.chain').addClass('enabled');
                                    settings.chainVideos = true;
                                }
                                else {
                                    settings.chainVideos = false;
                                }
                            }
                            settings.element.style.width = settings.width;
                            //settings.element.style.height = int(float(settings.width) / 1.777778);

                            list += '<div class="ytv-list-inner"><ul>';
                            var previous = '';
                            var next = '';
                            var thisVideo = settings.videoid;
                            for (i = 0; i < videos.length; i++) {
                                if (videos[i].media$group.yt$duration) {
                                    var video = {
                                        title: videos[i].title.$t,
                                        description: videos[i].media$group.media$description.$t,
                                        slug: videos[i].media$group.yt$videoid.$t,
                                        link: videos[i].link[0].href,
                                        published: videos[i].published.$t,
                                        rating: videos[i].yt$rating,
                                        stats: videos[i].yt$statistics,
                                        duration: (videos[i].media$group.yt$duration.seconds),
                                        thumb: videos[i].media$group.media$thumbnail[1].url
                                    };

                                    var date = new Date(null);
                                    date.setSeconds(video.duration);
                                    var timeSlots = (date.toTimeString().substr(0, 8)).split(':'),
                                        time = timeSlots[1] + ':' + timeSlots[2];

                                    var isFirst = '';
                                    if (settings.videoid == null && first === true) {
                                        thisVideo = video.slug;
                                        isFirst = ' class="ytv-active"';
                                        first = video;
                                        $('.ytv-button.prev').addClass('disabled');
                                    }
                                    else if (settings.videoid && video.slug == thisVideo) {
                                        if (i == videos.length - 1) {
                                            $('.ytv-button.next').addClass('disabled');
                                        }
                                        else if (i == 0) {
                                            $('.ytv-button.prev').addClass('disabled');
                                        }
                                        isFirst = ' class="ytv-active"';
                                        first = video;
                                    }
                                    if (i != 0 && thisVideo && video.slug == thisVideo) {
                                        previous = {
                                            title: videos[i - 1].title.$t,
                                            description: videos[i - 1].media$group.media$description.$t,
                                            slug: videos[i - 1].media$group.yt$videoid.$t,
                                            link: videos[i - 1].link[0].href,
                                            published: videos[i - 1].published.$t,
                                            rating: videos[i - 1].yt$rating,
                                            stats: videos[i - 1].yt$statistics,
                                            duration: (videos[i - 1].media$group.yt$duration.seconds),
                                            thumb: videos[i - 1].media$group.media$thumbnail[1].url
                                        };
                                        $('.ytv-button.prev').html('<a href="../Player/index.html?series=' + settings.playlist + '&video=' + previous.slug + '" data-ytv-title="' + (previous.title) + '" data-ytv="' + (previous.slug) + '" title="' + (previous.title) + '" class="ytv-clear">');
                                    }
                                    if (i != videos.length - 1 && thisVideo && video.slug == thisVideo) {
                                        next = {
                                            title: videos[i + 1].title.$t,
                                            description: videos[i + 1].media$group.media$description.$t,
                                            slug: videos[i + 1].media$group.yt$videoid.$t,
                                            link: videos[i + 1].link[0].href,
                                            published: videos[i + 1].published.$t,
                                            rating: videos[i + 1].yt$rating,
                                            stats: videos[i + 1].yt$statistics,
                                            duration: (videos[i + 1].media$group.yt$duration.seconds),
                                            thumb: videos[i + 1].media$group.media$thumbnail[1].url
                                        };
                                        $('.ytv-button.next').html('<a href="../Player/index.html?series=' + settings.playlist + '&video=' + next.slug + '" data-ytv-title="' + (next.title) + '" data-ytv="' + (next.slug) + '" title="' + (next.title) + '" class="ytv-clear">');
                                    }

                                    list += '<li' + isFirst + '><a href="../Player/index.html?series=' + settings.playlist + '&video=' + (video.slug) + '" data-ytv-title="' + (video.title) + '" data-ytv="' + (video.slug) + '" title="' + (video.description) + '" class="ytv-clear">';
                                    //list += '<div class="ytv-thumb"><div class="ytv-thumb-stroke"></div><span>'+(time)+'</span><img src="'+(video.thumb)+'"></div>';
                                    list += '<div class="ytv-content"><b>' + (video.title) + '</b>';
                                    if (video.stats) {
                                        list + '</b><span class="ytv-views">' + utils.addCommas(video.stats.viewCount) + ' Views</span>';
                                    }
                                    list += '</div></a></li>';
                                }
                            }
                            list += '</ul></div>';
                            settings.element.innerHTML = '<div class="ytv-video"><div id="ytv-video-player"></div></div>';
                            $('#video-list').html('<div class="ytv-list">' + list + '</div>');

                            //$('.ytv-list-inner').slimScroll({
                            //    color: '#939597',
                            //    opacity: 0.6,
                            //    wrapperClass: "list_scroll",
                            //    barClass: 'list_scroll_Bar',
                            //    railClass: 'list_scroll_rail',
                            //    position: 'right',
                            //    distance: 3,
                            //    size: 9,
                            //    width: '300px',
                            //    height: $('#player.ytv-canvas').height() + 'px',
                            //    alwaysVisible: true,
                            //    wheelStep: 5
                            //});

                            //$('.ytv-divider').click(function () {
                            //    $('.ytv-list').toggle();
                            //    if($('.ytv-list:hidden').length)
                            //    {
                            //        $('.ytv-divider').addClass('collapsed');
                            //        //$('.ytv-divider').css('right', '0px');
                            //        $('.ytv-video').css('right', '0px');
                            //        $('#player.ytv-canvas').height(554);
                            //    }
                            //    else {
                            //        $('.ytv-divider').removeClass('collapsed');
                            //        $('.ytv-video').css('right', '300px');
                            //        $('#player.ytv-canvas').height(380);
                            //    }
                            //});
                            var categoryButtons = "";
                            if (user.categories.length > 0) {
                                categoryButtons += '<div style="float:right;margin-left:8px;">';
                                for (i = 0; i < user.categories.length; i++) {
                                    categoryButtons += '<div class="ytv-button category"><a href="../../Videos/index.html?category=' + user.categories[i] + '">' + user.categories[i] + '</a></div>';
                                }
                                categoryButtons += '</div>';
                            }
                            $('#video-list-title').html(user.title + categoryButtons);
                            //$('#crumbs_page_title').html((user.title));
                            //settings.element.parentNode.insertBefore(listheaderelement, settings.element);

                            action.logic.loadVideo(first.slug, settings.autoplay, first.title, first.description);

                            if (settings.browsePlaylists) {
                                utils.ajax.get(utils.endpoints.userPlaylists(), prepare.playlists);
                            }

                        });
                    }
                }
            },
            action = {

                logic: {

                    playerStateChange: function (d) {
                        console.log(d);
                    },

                    loadVideo: function (slug, autoplay, title, description) {

                        var house = doc.getElementsByClassName('ytv-video')[0];
                        house.innerHTML = '<div id="ytv-video-player"></div>';
                        //$('#video_title').html(title);
                        $('title').html((title));
                        $('h1').html((title));
                        $('#crumbs_page_title').html((title));
                        $('.ytv-video-description').html(replaceLineBreaks(replaceURL(description)));
                        cache.player = new YT.Player('ytv-video-player', {
                            videoId: slug,
                            events: {
                                onReady: function (e) {
                                    e.target.setPlaybackQuality('hd1080');
                                    //var scrollTop = $('a[data-ytv=' + slug + ']').position().top;
                                    //$('.ytv-list-inner').slimScroll({
                                    //    color: '#939597',
                                    //    opacity: 0.6,
                                    //    wrapperClass: "list_scroll",
                                    //    barClass: 'list_scroll_Bar',
                                    //    railClass: 'list_scroll_rail',
                                    //    position: 'right',
                                    //    distance: 3,
                                    //    size: 9,
                                    //    width: '300px',
                                    //    height: $('#player.ytv-canvas').height() + 'px',
                                    //    alwaysVisible: true,
                                    //    wheelStep: 5,
                                    //    scrollBy: scrollTop
                                    //});
                                    settings.events.videoReady.call(this, e);
                                },
                                onStateChange: function (e) {
                                    var chain = readCookie('video_chain');
                                    if (chain != null) {
                                        if (chain == "true") {
                                            settings.chainVideos = true;
                                        }
                                        else {
                                            settings.chainVideos = false;
                                        }
                                    }
                                    if ((e.target.getPlayerState() === 0) && settings.chainVideos) {
                                        var ns = doc.getElementsByClassName('ytv-active')[0].nextSibling,
                                            link = ns.children[0];
                                        link.click();
                                    }
                                    if (e.data == YT.PlayerState.BUFFERING) {
                                        e.target.setPlaybackQuality('hd1080');
                                    }
                                    settings.events.stateChange.call(this, e);
                                }
                            },
                            playerVars: {
                                autohide: 1,
                                modestbranding: 1,
                                enablejsapi: 1,
                                origin: doc.domain,
                                controls: settings.controls ? 1 : 0,
                                rel: 0,
                                showinfo: 0,
                                iv_load_policy: settings.annotations ? '' : 3,
                                autoplay: autoplay ? 1 : 0,
                                wmode: settings.wmode
                            }
                        });

                    }

                },

                endpoints: {
                    videoClick: function (e) {
                        if (settings.async) {
                            var target = utils.parentUntil(e.target ? e.target : e.srcElement, 'data-ytv');

                            if (target) {

                                if (target.getAttribute('data-ytv')) {

                                    // Load Video
                                    utils.events.prevent(e);

                                    var activeEls = doc.getElementsByClassName('ytv-active'),
                                        i;
                                    for (i = 0; i < activeEls.length; i++) {
                                        activeEls[i].className = "";
                                    }
                                    target.parentNode.className = "ytv-active";
                                    action.logic.loadVideo(target.getAttribute('data-ytv'), true, target.getAttribute('data-ytv-title'), target.getAttribute('title'));

                                }

                            }
                        }
                    },
                    playlistToggle: function (e) {
                        var target = utils.parentUntil(e.target ? e.target : e.srcElement, 'data-ytv-playlist-toggle');

                        if (target && target.getAttribute('data-ytv-playlist-toggle')) {

                            // Toggle Playlist
                            utils.events.prevent(e);
                            var lh = doc.getElementsByClassName('ytv-list-header')[0];
                            if (lh.className.indexOf('ytv-playlist-open') === -1) {
                                lh.className += ' ytv-playlist-open';
                            } else {
                                lh.className = lh.className.replace(' ytv-playlist-open', '');
                            }
                        }
                    },
                    playlistClick: function (e) {
                        var target = utils.parentUntil(e.target ? e.target : e.srcElement, 'data-ytv-playlist');

                        if (target && target.getAttribute('data-ytv-playlist')) {

                            // Load Playlist
                            utils.events.prevent(e);

                            settings.playlist = target.getAttribute('data-ytv-playlist');
                            target.children[1].innerHTML = 'Loading...';

                            utils.ajax.get(utils.endpoints.playlistVids(), function (res) {
                                var lh = doc.getElementsByClassName('ytv-list-header')[0];
                                lh.className = lh.className.replace(' ytv-playlist-open', '');
                                prepare.compileList(res);
                            });
                        }
                    }
                },
                loadAds: function () {
                    utils.ajax.get(utils.endpoints.adPlaylist(), function (data) {
                        var videos = data.feed.entry,
                            i;
                        cache.ads = [];
                        for (i = 0; i < videos.length; i++) {
                            cache.ads.push({
                                title: videos[i].title.$t,
                                slug: videos[i].media$group.yt$videoid.$t,
                                duration: parseInt(videos[i].media$group.yt$duration.seconds, 10),
                            });
                        }
                    });
                },
                bindEvents: function () {

                    utils.events.addEvent(settings.element, 'click', action.endpoints.videoClick);
                    utils.events.addEvent(settings.element, 'click', action.endpoints.playlistToggle);
                    utils.events.addEvent(settings.element, 'click', action.endpoints.playlistClick);

                }
            },

            initialize = function (id, opts) {
                utils.deepExtend(settings, opts);
                settings.element = (typeof id === 'string') ? doc.getElementById(id) : id;
                if (settings.element && settings.user) {
                    prepare.youtube(function () {
                        prepare.build();
                        action.bindEvents();
                        utils.ajax.get(settings.playlist ? utils.endpoints.playlistVids() : utils.endpoints.userVids(), prepare.compileList);
                    });
                }
            };

        /*
         * Public
         */
        this.destroy = function () {
            utils.events.removeEvent(settings.element, 'click', action.endpoints.videoClick);
            utils.events.removeEvent(settings.element, 'click', action.endpoints.playlistToggle);
            utils.events.removeEvent(settings.element, 'click', action.endpoints.playlistClick);
            settings.element.className = '';
            settings.element.innerHTML = '';
        };
        this.fullscreen = {
            state: function () {
                return (settings.element.className).indexOf('ytv-full') !== -1;
            },
            enter: function () {
                if ((settings.element.className).indexOf('ytv-full') === -1) {
                    settings.element.className += 'ytv-full';
                }
            },
            exit: function () {
                if ((settings.element.className).indexOf('ytv-full') !== -1) {
                    settings.element.className = (settings.element.className).replace('ytv-full', '');
                }
            }
        };

        initialize(id, opts);
    };
    if ((typeof module !== 'undefined') && module.exports) {
        module.exports = YTV;
    }
    if (typeof ender === 'undefined') {
        this.YTV = YTV;
    }
    if ((typeof define === "function") && define.amd) {
        define("YTV", [], function () {
            return YTV;
        });
    }
    if ((typeof jQuery !== 'undefined')) {
        jQuery.fn.extend({
            ytv: function (options) {
                return this.each(function () {
                    new YTV(this, options);
                });
            }
        });
    }
}).call(this, window, document);
