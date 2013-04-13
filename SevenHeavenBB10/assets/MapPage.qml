import bb.cascades 1.0

Page {
    property alias title: mapTitleBar.title
    titleBar: TitleBar {
        id: mapTitleBar
    }
    ScrollView {
        scrollViewProperties.scrollMode: ScrollMode.Both
        scrollViewProperties.pinchToZoomEnabled: true
        Container {
            WebView {
                id: mapWebView
                url: "http://eco-bodhi.herokuapp.com"
                settings.zoomToFitEnabled: true
            }
        }
    }
}
