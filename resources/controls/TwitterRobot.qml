import QtQuick 2.5
import QtQml 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 1.4


Item {
    id:twitterRobot

    property string consumerKey : parameters.twitterKey
    property string consumerSecret : parameters.twitterSecret

    //property string consumerKey : "FJtUAm4gE9QypEDFI2PvibCkd"
    //property string consumerSecret : "dJy9nOnN3xH7EghWxhttRZHjQxPcogiIAJYsG54fkH8b2B8Qxx"

    property string bearerToken : ""
    property string lastTweetRetrieved : ""

    property variant model: tweets
    property string tweeterAccount : parameters.twitterAccount
    property string tweeterTag : parameters.twitterTag

    property int maxTwitterCount:1

    property int status: XMLHttpRequest.UNSENT
    property bool isLoading: status === XMLHttpRequest.LOADING
    property bool wasLoading: false

    signal tweetsLoaded

    ListModel { id: tweets }

    function connectToTwitter() {
        if (consumerKey === "" || consumerSecret === "") {
            console.debug("Twitter : No twitter id")
            return;
        }

        console.debug("Twitter account:" + consumerKey)
        console.debug("Twitter secret:" + consumerSecret)

        var authReq = new XMLHttpRequest;
        authReq.open("POST", "https://api.twitter.com/oauth2/token");
        authReq.setRequestHeader("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");
        authReq.setRequestHeader("Authorization", "Basic " + Qt.btoa(consumerKey + ":" + consumerSecret));
        authReq.onreadystatechange = function() {
            if (authReq.readyState === XMLHttpRequest.DONE) {
                var jsonResponse = JSON.parse(authReq.responseText);
                if (jsonResponse.errors !== undefined)
                    console.log("Authentication error: " + jsonResponse.errors[0].message)
                else
                    bearerToken = jsonResponse.access_token;
                    console.debug("Twitter : connected with account")
            }
        }
        authReq.send("grant_type=client_credentials");
    }

    //Recherche de nouveaux Tweets. Complète la liste "tweets"
    function reload() {
        console.log("Reload Tweets")
        tweets.clear()

        if (tweeterAccount == "" && tweeterTag == "")
            return;
//Building Query
        var query = "https://api.twitter.com/1.1/search/tweets.json?" +
//                    "result_type=recent" +
                    "&count=" + maxTwitterCount +
                    "&q=";
        if (tweeterAccount != "") {
            query += encodeURIComponent(tweeterAccount);
        }
        if (tweeterAccount != "" && tweeterTag != "") {
            query += "+"
        }
        if (tweeterTag != "") {
            query += encodeURIComponent(tweeterTag);
        }

        //Ne recherche que les photos
        query += encodeURIComponent(" filter:images")

        if (lastTweetRetrieved != "") {
            query += "&since_id=" + lastTweetRetrieved
        }

        console.debug("Twitter Query : " + query);
        //return;

//! [requesting]
        var req = new XMLHttpRequest;
        req.open("GET", query);
        req.setRequestHeader("Authorization", "Bearer " + bearerToken);
        req.onreadystatechange = function() {
            status = req.readyState;
            if (status === XMLHttpRequest.DONE) {
                var objectArray = JSON.parse(req.responseText);
                if (objectArray.errors !== undefined)
                    console.log("Error fetching tweets: " + objectArray.errors[0].message)
                else {
                    console.debug("Last tweet : " + objectArray.search_metadata.max_id_str)
                    //Save last tweet to get only new tweets
                    lastTweetRetrieved = objectArray.search_metadata.max_id_str
                    for (var key in objectArray.statuses) {
                        var jsonObject = objectArray.statuses[key];
                        tweets.append(jsonObject);
                    }
                }
                if (wasLoading == true)
                    twitterRobot.tweetsLoaded()
            }
            wasLoading = (status === XMLHttpRequest.LOADING);
        }
        req.send();
//! [requesting]
    }

    //Pour tout les nouveaux tweets avec une photo, créer un visuel
    onTweetsLoaded: {
        console.debug("Number of new tweets : " + tweets.count);

        for (var i = 0; i < tweets.count; i++) {
            var date = tweets.get(i).created_at
            var id = tweets.get(i).id
            var text = tweets.get(i).text
            var profile_name = tweets.get(i).user.name
            var profile_image = tweets.get(i).user.profile_image_url
            var media_url = tweets.get(i).entities.media[0].media_url

            console.log("Date:" + date + "\nid:" + id + "\ntext:" + text +
                        "\nname:" + profile_name + "\navatar:" + profile_image + "\nmedia:" + media_url + "\n\n")

            parameters.photoGallery.addPhoto("photo1_tweet", globalVar.currentTemplate, text, profile_name, media_url)
        }
    }

    Component.onCompleted: {
        connectToTwitter();
    }

    onConsumerKeyChanged: {
        //connectToTwitter();
    }

    onConsumerSecretChanged: {
        //connectToTwitter();
    }

    //Cherche toute les 10 secondes un nouveau tweet si connecté à tweeter et ecoute de tweeter
    Timer {
        interval: 15000;
        running: (parameters.twitterListenTwitter && (bearerToken !== ""))
        repeat: true;
        onTriggered: {
            console.debug("Recherche nouveau tweets")
            reload()
        }
    }
}
