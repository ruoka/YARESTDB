#include <gtest/gtest.h>
#include "http/server.hpp"

namespace view {

const auto index =
u8R"(

<!DOCTYPE html>
<html>
<head>
    <title>MVC++</title>
    <style type="text/css">
    </style>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.2/jquery.min.js"></script>
    <script type="text/javascript">
    $(document).ready(function() {
        $("button").click( function() {
            $.ajax({url : $(this).attr("data-url"), method : $(this).attr("data-method")})
            .done(function(data) {$("#content").html(data)})
            .fail(function (jqXHR, textStatus, errorThrown) {alert(textStatus + ":" + jqXHR.status)})
        })
    })
    </script>
</head>
<body>
    <aside>
        <button type="button" data-url="/vk" data-method="GET"   > GET</button>
        <button type="button" data-url="/vk" data-method="HEAD"  > HEAD</button>
        <button type="button" data-url="/vk" data-method="POST"  > POST</button>
        <button type="button" data-url="/vk" data-method="PUT"   > PUT</button>
        <button type="button" data-url="/vk" data-method="PATCH" > PATCH</button>
        <button type="button" data-url="/vk" data-method="DELETE"> DELETE</button>
        <button type="button" data-url="/json" data-method="GET" > JSON</button>
    </aside>
    <article id = "content"></article>
</body>
</html>

)"s;

const auto get = u8R"(<p>GET response</p>)"s;

const auto head = u8R"(<p>HEAD response</p>)"s;

const auto post = u8R"(<p>POST response</p>)"s;

const auto put = u8R"(<p>PUT response</p>)"s;

const auto destroy = u8R"(<p>DELETE response</p>)"s;

const auto patch = u8R"(<p>PATCH response</p>)"s;

const auto json = u8R"( {"foo" : 1, "bar" : false} )"s;

} // namespace view

TEST(HttpServer,Setup)
{
    auto server = http::server{};

    server.get("/"s).response(view::index);
    server.get("/vk"s).response(view::get);
    server.head("/vk"s).response(view::head);
    server.post("/vk"s).response(view::post);
    server.put("/vk"s).response(view::put);
    server.patch("/vk"s).response(view::patch);
    server.destroy("/vk"s).response(view::destroy);
    server.get("/json"s).response(view::json);

    server.listen("8080"s);
}
