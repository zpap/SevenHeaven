$(document).ready(function() {
    var dataSource = new kendo.data.DataSource({
        transport: {
            read: {
                url: "http://localhost:5000/facts",
                contentType: "application/json; charset=utf-8",
                type: "GET",
                dataType: "json",
                data: {}
            }
        },
        schema: {
            data: "data",
            total: "total"
        }
    });

    $("#factsListView").kendoListView({
        dataSource: dataSource,
        template: kendo.template($("#factsTemplate").html())
    });
});