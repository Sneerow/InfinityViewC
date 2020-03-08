// Loader design
let loader =  "<div class='ui active dimmer'><div class='ui indeterminate text loader'>Preparing Files</div></div><p></p>";


// When loaded
$(document).ready(() => {
    $.get("./templates/html/home.html", function(data) {
        $('.view').empty().append(data);

    })
});

// Button manager

// Home buttom
$(document).on("click", ".homeButton", function(event) {
    console.log('test');
    event.preventDefault();
    $(".view").empty().append(loader);
    $(".active").removeClass('active');
    $(".active").removeClass('active');
    $("#homeButton1").addClass("active");
    $("#homeButton2").addClass("active");

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
    
    $.get("./templates/html/discovery.html", function(data) {
        $('.view').empty().append(data);

    });

});
