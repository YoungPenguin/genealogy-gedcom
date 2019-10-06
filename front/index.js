/*
Front end for the praser-
The database is commented out around line 500
*/
var loginStatus = false;
const DEBUG = true;

// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    console.log("page has been loaded");
    var listOfFileNames = [];

    //ajax get the list of file names
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileList',
        success: function (data) {
            console.log("file name = " + data);
            listOfFileNames = data;
            for(x = 0; x<listOfFileNames.length; x++){
                var currentFileName = "<option >"+listOfFileNames[x]+"</option>";
                $("select").not("#selectQueryID").append(currentFileName);
            }
        },
        fail: function(error) {
            console.log(error);
        }
    });

    //Loading and prasing the table with files from the GEDCOMfiles folder
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileLogs',
        success: function (data) {
            console.log("file logs = " + data);
            $(".fileLogTable tbody").empty(); // removing the N/A
            for(var x = 0; x<data.length; x++){
                var gedFileNameStringOnly = data[x].fileName.substring(10, data[x].fileName.length);
                var tableSections  = "<tbody><tr>"
                    +"<td><a class=\"setLightBlue\" href=\"" + data[x].fileName +"\">" + gedFileNameStringOnly + "</a></td>"
                    +"<td>" + data[x].source + "</td>"
                    +"<td>" + data[x].gedcVersion + "</td>"
                    +"<td>" + data[x].encoding + "</td>"
                    +"<td>" + data[x].subName + "</td>"
                    +"<td>" + data[x].subAddress + "</td>"
                    +"<td>" + data[x].indiNum + "</td>"
                    +"<td>" + data[x].famNum + "</td>"
                    +"</tr></tbody>"
                    $(".fileLogTable").append(tableSections);
            }
            if(data.length == 0 || isEmptyObject(data) == true){
                var tableSections  = "<tbody><th>NA</th></tr></tbody>"
                $(".fileLogTable").append(tableSections);
            }
        },
        fail: function(error) {
            console.log(error);
        }
    });

    //creat a gedcom and adding it to the html table, from here you can add individual
    $('.createGedcom').on('click', function(event) {
        console.log("createGedcom");
        var fileName = $(".fileNameCreateGed").val();
        var subName = $(".subNameCreateGed").val();
        var subAddress = $(".subAddressCreateGed").val();
        console.log("create ged form: " + fileName + subName + subAddress);
        appendStringToStatus("Creating a GEDCOM file...");
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/createGedcom',
            data: {fileName: fileName, subName: subName, subAddress: subAddress},
            success: function (data) {
                console.log("file logs = " + data);
                var tableSections  = "<tbody><tr>"
                    +"<td><a class=\"setLightBlue\" href=\"./GEDCOMfiles/" + fileName +".ged\">" + fileName + ".ged</a></td>"
                    +"<td>" + "Ancestry.com" + "</td>"
                    +"<td>" + "5.5" + "</td>"
                    +"<td>" + "ASCII" + "</td>"
                    +"<td>" + subName + "</td>"
                    +"<td>" + subAddress + "</td>"
                    +"<td>" + "0" + "</td>"
                    +"<td>" + "0" + "</td>"
                    +"</tr></tbody>"
                $(".fileLogTable").append(tableSections);
            },
            fail: function(error) {
                console.log(error);
            }
        });
        var emptyString = "";
        $(".fileNameCreateGed").val(emptyString);
        $(".subNameCreateGed").val(emptyString);
        $(".subAddressCreateGed").val(emptyString);
        $("select option").remove();
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/getFileList',
            success: function (data) {
                console.log("file name = " + data);
                listOfFileNames = data;
                for(x = 0; x<listOfFileNames.length; x++){
                    var currentFileName = "<option >"+listOfFileNames[x]+"</option>";
                    $("select").append(currentFileName);
                }
            },
            fail: function(error) {
                console.log(error);
            }
        });
        appendStringToStatus("GEDCOM \"" + fileName +"\" has been created.");
    });

    // Now we need to be able to add individuals to the newly created GEDCOM file
    $('.addIndividual').on('click', function(event){
        console.log("calling ajax selection menu");
        var element = document.getElementById('indiFileSelection');
        var fileSelected = element.options[element.selectedIndex].text;
        console.log("file selected: " + fileSelected);

        var emptyString = "";
        var firstName = "";
        var lastName = "";
        firstName = $(".addIndiFirstName").val();
        lastName = $(".addIndiLastname").val();

        console.log("firstName = " + firstName);
        console.log("lastName = " + lastName);

        if(firstName.length > 0 && lastName.length > 0){
            $.ajax({
                type: 'get',
                dataType: 'json',
                url: '/addIndiToList',
                data: {fileSelected: fileSelected, givenName: firstName, surname: lastName},
                success: function (data) {
                    console.log("addIndiToList = (void?) " + data);
                    appendStringToStatus("Adding " + givenName + " " + surname + " to the list of individual in " + fileSelected + ".");
                },
                fail: function(error) {
                    console.log(error);
                }
            });


            $(".addIndiFirstName").val(emptyString);
            $(".addIndiLastname").val(emptyString);

            console.log("calling ajax selection menu");
            var element = document.getElementById('gedcomFileSelection');
            var fileSelected = element.options[element.selectedIndex].text;
            console.log("file selected: " + fileSelected);
            $(".indiTable tbody").remove();
            $.ajax({
                type: 'get',
                dataType: 'json',
                url: '/getIndiList',
                data: {fileSelected: fileSelected},
                success: function (data) {
                    console.log("getIndiList object = " + data);
                    for(var x = 1; x<data.length; x++){
                        var tableSections  = "<tbody><tr>"
                            +"<td>" + x + "</td>"
                            +"<td>" + data[x].givenName + "</td>"
                            +"<td>" + data[x].surname + "</td>"
                            +"<td>" + data[x].sex + "</td>"
                            +"<td>" + data[x].famNum + "</td>"
                            +"</tr></tbody>"
                        $(".indiTable").append(tableSections);
                    }
                },
                fail: function(error) {
                    console.log(error);
                }
            });
        }else{
            appendStringToStatus("Failed Adding to the list of individual in " + fileSelected.substring(10, fileSelected.length) + ".");
            appendStringToStatus("First name and last name must be entered to add individual.");
        }
    });

    $('.setAnimateScroll').on('click', function(event) {
        if (this.hash !== "") {
            event.preventDefault();
            var hash = this.hash;
            $('html, body').animate({
                scrollTop: $(hash).offset().top
            }, 1000, function(){
                window.location.hash = hash;
                console.log("calling animate scroll");
            });
        }
    });

    $('.gedcomFileSelection').change('click', function(event){
        console.log("calling ajax selection menu");
        var element = document.getElementById('gedcomFileSelection');
        var fileSelected = element.options[element.selectedIndex].text;
        console.log("file selected: " + fileSelected);
        $(".indiTable tbody").remove();
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/getIndiList',
            data: {fileSelected: fileSelected},
            success: function (data) {
                console.log("getIndiList object = " + data);
                for(var x = 0; x<data.length; x++){
                    var tableSections  = "<tbody><tr>"
                        +"<td>" + x + "</td>"
                        +"<td>" + data[x].givenName + "</td>"
                        +"<td>" + data[x].surname + "</td>"
                        +"<td>" + data[x].sex + "</td>"
                        +"<td>" + data[x].famNum + "</td>"
                        +"</tr></tbody>"
                    $(".indiTable").append(tableSections);
                }
            },
            fail: function(error) {
                console.log(error);
            }
        });
    });

    $('.searchDesc').on('click', function(event){
        console.log("calling ajax selection menu");
        var element = document.getElementById('descFileSelection');
        var fileSelected = element.options[element.selectedIndex].text;
        console.log("file selected: " + fileSelected);
        var givenName = $(".firstNameInputDesc").val();
        var surname = $(".lastNameInputDesc").val();
        var numGen = $(".numGenInputDesc").val();
        $(".genTable tbody").remove();
        if(numGen < 0){
            appendStringToStatus("Failed Searching for descendants, number of generation must be greater than 0.");
            var genNum = 1;
            var info = "Invalid input, please re-enter or continue...";
            var tableSections  = "<tbody><tr>"
                +"<td>" + genNum + "</td>"
                +"<td>" + info + "</td>"
                +"</tr></tbody>"
            $(".genTable").append(tableSections);
            return;
        }

// getting the list of descentend from the selected file
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/getDescList',
            data: {fileSelected: fileSelected, givenName: givenName, surname: surname, numGen: numGen},
            success: function (data) {
                console.log("get gen list object = " + data);
                for(var x = 0; x<data.length; x++){
                    var indiListString = "";
                    for(y = 0; y<data[x].length; y++){
                        var commaOrPeriod;
                        if(y==data[x].length-1){
                            commaOrPeriod = "."
                        }else{
                            commaOrPeriod = ", "
                        }
                        indiListString = indiListString + data[x][y].givenName + " " + data[x][y].surname + commaOrPeriod;
                    }
                    console.log("indi list = " + indiListString);
                    var genNum = x + 1;
                    var tableSections  = "<tbody><tr>"
                        +"<td>" + genNum + "</td>"
                        +"<td>" + indiListString + "</td>"
                        +"</tr></tbody>"
                    $(".genTable").append(tableSections);
                }

                if(isEmptyObject(data) == true){
                    var genNum = 1;
                    var info = "NA";
                    var tableSections  = "<tbody><tr>"
                        +"<td>" + genNum + "</td>"
                        +"<td>" + info + "</td>"
                        +"</tr></tbody>"
                    $(".genTable").append(tableSections);
                }
            },
            fail: function(error) {
                console.log(error);
            }
        });
        appendStringToStatus("Searching " + numGen +" generation of " + givenName + " " + surname + "'s descendants.");
        var emptyString = "";
        $(".firstNameInputDesc").val(emptyString);
        $(".lastNameInputDesc").val(emptyString);
        $(".numGenInputDesc").val(emptyString);
    });

    $('.searchAnce').on('click', function(event){
        console.log("calling ajax selection menu");
        var element = document.getElementById('anceFileSelection');
        var fileSelected = element.options[element.selectedIndex].text;
        console.log("file selected: " + fileSelected);
        var givenName = $(".firstNameInputAnce").val();
        var surname = $(".lastNameInputAnce").val();
        var numGen = $(".numGenInputAnce").val();
        $(".genTable tbody").remove();
        if(numGen < 0){
            appendStringToStatus("Failed Searching for ancestors, number of generation must be greater than 0.");
            var genNum = 1;
            var info = "Invalid input, please re-enter or continue...";
            var tableSections  = "<tbody><tr>"
                +"<td>" + genNum + "</td>"
                +"<td>" + info + "</td>"
                +"</tr></tbody>"
            $(".genTable").append(tableSections);
            return;
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/getAnceList',
            data: {fileSelected: fileSelected, givenName: givenName, surname: surname, numGen: numGen},
            success: function (data) {
                console.log("get gen list object = " + data);
                for(var x = 0; x<data.length; x++){
                    var indiListString = "";
                    for(y = 0; y<data[x].length; y++){
                        var commaOrPeriod;
                        if(y==data[x].length-1){
                            commaOrPeriod = "."
                        }else{
                            commaOrPeriod = ", "
                        }
                        indiListString = indiListString + data[x][y].givenName + " " + data[x][y].surname + commaOrPeriod;
                    }
                    console.log("indi list = " + indiListString);
                    var genNum = x + 1;
                    var tableSections  = "<tbody><tr>"
                        +"<td>" + genNum + "</td>"
                        +"<td>" + indiListString + "</td>"
                        +"</tr></tbody>"
                    $(".genTable").append(tableSections);
                }

                if(isEmptyObject(data) == true){
                    var genNum = 1;
                    var info = "NA";
                    var tableSections  = "<tbody><tr>"
                        +"<td>" + genNum + "</td>"
                        +"<td>" + info + "</td>"
                        +"</tr></tbody>"
                    $(".genTable").append(tableSections);
                }
            },
            fail: function(error) {
                console.log(error);
            }
        });
        appendStringToStatus("Searching " + numGen +" generation of " + givenName + " " + surname + "'s ancestors.");
        var emptyString = "";
        $(".firstNameInputAnce").val(emptyString);
        $(".lastNameInputAnce").val(emptyString);
        $(".numGenInputAnce").val(emptyString);
    });

    $('#describeIndiID').click(function(){
        var helpCommand = "DESCRIBE INDIVIDUAL;";
        var jqueryID = '#textAreaQueryID';
        $(jqueryID).val(helpCommand);
    });

    $('#describeFileID').click(function(){
        var helpCommand = "DESCRIBE FILE;";
        var jqueryID = '#textAreaQueryID';
        $(jqueryID).val(helpCommand);
    });
});

function appendToTable(data, toBeAppend){
    console.log("calling appendToQueryTable");
    var thead = toBeAppend + " thead";
    var tbody = toBeAppend + " tbody";
    $(thead).remove();
    $(tbody).remove();

    var header = "";
    var headerList = "";

    var keys = Object.keys(data[0]);
    console.log("keys = " + keys);
    for(var x=0; x<keys.length; x++){
        var headerList = headerList + "<th>"+ keys[x] +"</th>";
    }
    header = "<thead><tr>" + headerList + "</tr></thead>";
    $(toBeAppend).append(header);

    for(var x=0; x<data.length; x++){
        var body = "";
        var bodyList = "";
        var values = Object.values(data[x]);
        console.log("values = " + values);
        for(var y=0; y<values.length; y++){
            bodyList = bodyList + "<th>" + values[y] + "</th>";
        }
        body = "<tbody><tr>"+ bodyList + "</tr></tbody>";
        $(toBeAppend).append(body);
    }
}

function appendIndiTable(data, toBeAppend){
    console.log("calling appendIndiTable");
    var thead = toBeAppend + "thead";
    var tbody = toBeAppend + "tbody";
    $(thead).remove();
    $(tbody).remove();
    var headerHtml = toBeAppend
        + "<thead>"
        + "<tr>"
        + "<th>#</th>"
        + "<th>Given Name</th>"
        + "<th>Surname</th>"
        + "<th>Sex</th>"
        + "<th>Family Size</th>"
        + "</tr>"
        + "</thead>"
    &(toBeAppend).append(headerHtml);
    for(var x = 1; x<data.length; x++){
        var tableSections  = "<tbody><tr>"
            +"<td>" + x + "</td>"
            +"<td>" + data[x].givenName + "</td>"
            +"<td>" + data[x].surname + "</td>"
            +"<td>" + data[x].sex + "</td>"
            +"<td>" + data[x].famNum + "</td>"
            +"</tr></tbody>"
        $(toBeAppend).append(tableSections);
    }
}

function appendFileTable(data, toBeAppend){
    console.log("calling appendFileTable");
    var thead = toBeAppend + "thead";
    var tbody = toBeAppend + "tbody";
    $(thead).remove();
    $(tbody).remove();
    var headerHtml = toBeAppend
        + "<thead>"
        + "<tr>"
        + "<th>File Name</th>"
        + "<th>Source</th>"
        + "<th>GEDCOM Version</th>"
        + "<th>Encoding</th>"
        + "<th>Submitter Name</th>"
        + "<th>Submitter Address</th>"
        + "<th>Number of Individuals</th>"
        + "<th>Number of Families</th>"
        + "</tr>"
        + "</thead>";
    $(toBeAppend).append(headerHtml);
    for(var x = 0; x<data.length; x++){
        var gedFileNameStringOnly = data[x].fileName.substring(10, data[x].fileName.length);
        var tableSections  = "<tbody><tr>"
            +"<td><a class=\"setLightBlue\" href=\"" + data[x].fileName +"\">" + gedFileNameStringOnly + "</a></td>"
            +"<td>" + data[x].source + "</td>"
            +"<td>" + data[x].gedcVersion + "</td>"
            +"<td>" + data[x].encoding + "</td>"
            +"<td>" + data[x].subName + "</td>"
            +"<td>" + data[x].subAddress + "</td>"
            +"<td>" + data[x].indiNum + "</td>"
            +"<td>" + data[x].famNum + "</td>"
            +"</tr></tbody>";
        $(toBeAppend).append(tableSections);
    }
}

function fileInfoAjax(){
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbQueryOuputs',
        success: function (data) {
            console.log("ajax pass");
            console.log("fileNum = " + data.fileNum + ", indiNum = " + data.indiNum);
            appendStringToStatus(printDBstatus(data.fileNum, data.indiNum));
        },
        fail: function(error) {
            console.log("ajax error returned = " + error);
        }
    });
}

function appendStringToStatus(string){
    var statusString = "-> " + string + '\n';
    var myTextArea = $('#statusTextAreaID');
    myTextArea.val(myTextArea.val() + statusString);
}

function isEmptyObject(obj) {
    for(var prop in obj) {
        if (Object.prototype.hasOwnProperty.call(obj, prop)) {
            return false;
        }
    }
    return true;
}

function printDBstatus(numData, numIndi){
    return "Database has " + numData + " files and " + numIndi + " individuals";
}

/* this section is used for the id to the database, but its under conversion
    $('#loginButtonID').click(function(event){
        console.log("calling loginID jquery");
        var user = $('#userID').val();
        var pass = $('#passID').val();
        var dbase = $('#dbaseID').val();
        console.log("user = " + user + ", passID = " + pass + ", dbase = " + dbase);
        var connectionFail = true;
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/login',
            data: {user: user, pass: pass, dbase: dbase},
            success: function (data) {
                console.log("ajax pass returned = " + data);
                connectionFail = data;
                if(connectionFail == true){
                    alert("Invalid login");
                }else{
                    alert("Login successfull");
                    //unlock data base
                    appendStringToStatus("Successfull login as '"+ user+ "'");
                    //disable the login button
                    var loginElement = document.getElementById('loginNavBarID');
                    loginElement.innerText = user;
                    loginElement.href = "#";
                    loginStatus = true;
                }
            },
            fail: function(error) {
                console.log("ajax error returned = " + error);
            }
        });
        //empty the inputs
        var empty = "";
        $('#userID').val(empty);
        $('#passID').val(empty);
        $('#dbaseID').val(empty);
    });//end  jquery
*/
    /*******************************************************************/

    /*
    $('#storeAllFilesID').click(function(event){
        console.log("calling storeAllFilesID");
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/dbStoreFile',
            success: function (data) {
                console.log("ajax pass");
            },
            fail: function(error) {
                console.log("ajax error returned = " + error);
            }
        });
        fileInfoAjax();
        event.preventDefault();
    });
    //clear all files
    $('#clearAllDataID').click(function(event){
        console.log("calling storeAllFilesID");
        var queryFail = true;
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/dbClearFile',
            success: function (data) {
                console.log("ajax pass");
            },
            fail: function(error) {
                console.log("ajax error returned = " + error);
            }
        });
        fileInfoAjax();
        event.preventDefault();
    });
    //execute query
    $('#executeQueryID').click(function(event){
        console.log("calling textAreaQueryID");
        var input = $('#textAreaQueryID').val();
        console.log("**input = " + input);
        fileInfoAjax();
        executeQueryAjax(input);
        var emptyString = "";
        $('#textAreaQueryID').val(emptyString);
    });
    //select query
    $('#selectQueryID').change('click', function(event){
        var emptyString = "";
        var jqueryID = '#textAreaQueryID';
        console.log("calling selectQueryID");
        var element = document.getElementById('selectQueryID');
        var querySelected = element.options[element.selectedIndex].text;
        console.log("file selected text = " + querySelected);
        if(querySelected == "Get all individuals sorted by last name"){
            var command = "SELECT * FROM INDIVIDUAL ORDER BY surname;";
            $(jqueryID).val(command);
        }else if(querySelected == "Get the individuals from a specific file"){
            var href = "#queryPopupID";
            window.location = href;
            $('#queryPopupButtonID').on('click', function(){
                console.log("calling query ok button");
                var fileElement = document.getElementById('queryFileSelection');
                var fileSelected = "";
                fileSelected = fileElement.options[fileElement.selectedIndex].text;
                var command = "SELECT * FROM INDIVIDUAL WHERE source_file = (SELECT file_id FROM FILE WHERE file_Name = \""+fileSelected +"\");";
                $(jqueryID).val(command);
            });
        }else if(querySelected == "Get the total number of female"){
            var command = "SELECT COUNT(*) FROM INDIVIDUAL WHERE sex = 'f';";
            $(jqueryID).val(command);
        }else if(querySelected == "Get the submitter of all individuals"){
            var command = "SELECT FILE.sub_name, INDIVIDUAL.given_name, INDIVIDUAL.surname FROM INDIVIDUAL JOIN FILE WHERE INDIVIDUAL.source_file = FILE.file_id ORDER BY FILE.sub_name;";
            $(jqueryID).val(command);
        }else if(querySelected == "Get the number of individuals of a specific file"){
            var href = "#queryPopupID";
            window.location = href;
            $('#queryPopupButtonID').on('click', function(){
                console.log("calling query ok button");
                var fileElement = document.getElementById('queryFileSelection');
                var fileSelected = "";
                fileSelected = fileElement.options[fileElement.selectedIndex].text;
                var command = "SELECT COUNT(*) FROM INDIVIDUAL WHERE source_file = (SELECT file_id FROM FILE WHERE file_Name = \""+fileSelected +"\");";
                $(jqueryID).val(command);
            });
        }
    });
    function executeQueryAjax(input){
        console.log("calling executeQueryAjax");
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/dbQueryInputs',
            data: {input: input},
            success: function (data) {
                console.log("ajax pass");
                console.log("ajax data = " + data);
                appendToTable(data, '#queryTableID');
            },
            fail: function(error) {
                console.log("ajax error returned = " + error);
            }
        });
    }
*/
    //help for indi