sap.ui.define([
    "sap/ui/core/mvc/Controller",
    "sap/ui/model/json/JSONModel"
], function (Controller, JSONModel) {
    "use strict";

    return Controller.extend("gonzalo123.iot.controller.Main", {
        model: new JSONModel({
            value: undefined
        }),

        onInit: function () {
            var model = this.model;
            var socket = window.io("my-host.cfapps.eu10.hana.ondemand.com");

            socket.on("value", function (msg) {
                model.setProperty("/value", msg);
            });

            this.getView().setModel(this.model, "view");
        },

        onAfterRendering: function () {
            var model = this.model;

            this.getView().getModel().read("/my-hana-table-odata-uri", {
                urlParameters: {
                    $top: 1,
                    $orderby: "G_CREATED desc"
                },
                success: function (oData) {
                    model.setProperty("/value", oData.results[0].C_VALUE);
                }
            });
        }
    });
});