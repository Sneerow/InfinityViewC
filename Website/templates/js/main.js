// When loaded
$(document).ready(() => {
    $.get("./templates/html/home.html", function(data) {
        $('.view').empty().append(data);

    })
});

// Button manager

// Loader
// Loader design
let loader =  "<div class='ui active dimmer'><div class='ui indeterminate text loader'>Preparing Files</div></div>";
function loader_append(where)
{
    $(where).append(loader);
}

// Home buttom
$(document).on("click", ".homeButton", function(event) {
    console.log('test');
    event.preventDefault();
    $(".view").empty().append(loader);
    $(".active").removeClass('active');
    $(".active").removeClass('active');
    $("#homeButton1").addClass("active");
    $("#homeButton2").addClass("active");

    loader_append(".view");

    $.get("./templates/html/home.html", function(data) {
        $('.view').empty().append(data);

    });

});

// Contacts Button
$(document).on("click", ".contactsButton", function(event) {
    console.log('test');
    event.preventDefault();
    $(".view").empty().append(loader);
    $(".active").removeClass('active');
    $(".active").removeClass('active');
    $("#contactsButton").addClass("active");
    
    loader_append(".view");

    $.get("./templates/html/contacts.html", function(data) {
        $('.view').empty().append(data);

    });

});

// Downloads Button
$(document).on("click" , ".downloadsButton", function(event) {
    console.log('test');
    event.preventDefault();
    $(".view").empty().append(loader);
    $(".active").removeClass('active');
    $(".active").removeClass('active');
    $("#downloadsButton").addClass("active");
    
    loader_append(".view");

    $.get("./templates/html/downloads.html", function(data) {
        $('.view').empty().append(data);

    });

});

// Discovery button
$(document).on("click" , ".discoveryButton", function(event) {
    console.log('test');
    event.preventDefault();
    $(".view").empty().append(loader);
    $(".active").removeClass('active');
    $(".active").removeClass('active');
    $("#discoveryButton").addClass("active");
    
    loader_append(".view");

    $.get("./templates/html/discovery.html", function(data) {
        $('.view').empty().append(data);

    });

});
