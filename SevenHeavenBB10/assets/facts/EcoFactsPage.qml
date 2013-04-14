import bb.cascades 1.0
import eco.bodhi 1.0
import "../tips"

Page {
    property alias title: factsTitleBar.title
    titleBar: TitleBar {
        id: factsTitleBar
    }
    Container {
        ListView {
            id: factsList
            dataModel: SimpleDataController {
                id: factsDataModel
            }
            listItemComponents: [
                ListItemComponent {
                    Container {
                        TipItem {}
                    }
                }
            ]
        }
        onCreationCompleted: {
            ecoBodhi.loadData.connect(loadDataModels);
            loadDataModels();
        }

        function loadDataModels() {
            // populate library list view model with all shows from library
            factsDataModel.load("data/feeds/facts.json");
        }
    }
}
