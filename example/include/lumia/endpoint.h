#pragma once

#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_

#include <iostream>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>


typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
//typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
typedef client::connection_ptr connection_ptr;



/*

Low level ws client implementation ; 
it forward all events to lumia
*/
class Endpoint {

public:
    
    typedef Endpoint type;

    Endpoint() {
        m_endpoint.set_access_channels(websocketpp::log::alevel::none);
        m_endpoint.set_error_channels(websocketpp::log::elevel::none);

        // Initialize ASIO
        m_endpoint.init_asio();

        // Register our handlers
       // m_endpoint.set_socket_init_handler(bind(&type::on_socket_init, this, ::_1));
        //m_endpoint.set_tls_init_handler(bind(&type::on_tls_init,this,::_1));
        m_endpoint.set_message_handler(bind(&type::on_message, this, ::_1, ::_2));
        m_endpoint.set_open_handler(bind(&type::on_open, this, ::_1));
        m_endpoint.set_close_handler(bind(&type::on_close, this, ::_1));
        m_endpoint.set_fail_handler(bind(&type::on_fail, this, ::_1));
    }

    void setCB( std::function<void()> o, std::function<void(int, int)> c, std::function<void(const std::string&)> m, std::function<void(const std::string&, int, int)> f) {
        o_ = o;
        c_ = c;
        m_ = m;
        f_ = f;
    }

    int start(std::string uri) {
       websocketpp::lib::error_code ec;
       client::connection_ptr con = m_endpoint.get_connection(uri, ec);

       if (ec) {
            m_endpoint.get_alog().write(websocketpp::log::alevel::app, ec.message());
            return 1;
       }

        //con->set_proxy("http://humupdates.uchicago.edu:8443");

        m_endpoint.connect(con);

        // Start the ASIO io_service run loop
        m_endpoint.run();

        m_endpoint.reset();

        return 0;
    }

    void stop() {
        m_endpoint.close(hdl_, websocketpp::close::status::going_away, "");
    }

    void on_socket_init(websocketpp::connection_hdl) {
        
    }

    /*
    context_ptr on_tls_init(websocketpp::connection_hdl) {
     
        context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);

        try {
            ctx->set_options(boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::no_sslv3 |
                boost::asio::ssl::context::single_dh_use);
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        return ctx;
    }
    */

    void on_fail(websocketpp::connection_hdl hdl) {
        client::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);
        int code = con->get_local_close_code();
        int code_r = con->get_remote_close_code();
        std::cout << "Fail handler" << std::endl;
        std::cout << con->get_state() << std::endl;
        std::cout << con->get_local_close_code() << std::endl;
        std::cout << con->get_local_close_reason() << std::endl;
        std::cout << con->get_remote_close_code() << std::endl;
        std::cout << con->get_remote_close_reason() << std::endl;
        std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
        if ( code == 1000 || code == 1001 || code == 1005 ) {
            f_("closed by us", code, code_r);
        }
        else {
            f_(con->get_local_close_reason(), code, code_r);
        }
        
    }

    void on_open(websocketpp::connection_hdl hdl) {
        hdl_ = hdl;
        o_();
       
    }
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
        m_(msg->get_payload());
    }
    void on_close(websocketpp::connection_hdl hdl) {
        client::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);
        c_(con->get_local_close_code() , con->get_remote_close_code());
    }

    void sendText(std::string& text) {
        m_endpoint.send(hdl_,text, websocketpp::frame::opcode::text);
    }

private:
    client m_endpoint;
    websocketpp::connection_hdl hdl_;
    std::function<void()> o_;
    std::function<void(int, int)> c_;
    std::function<void(const std::string&)> m_;
    std::function<void(const std::string&, int, int)> f_;
    bool stoped_ = true;

};
