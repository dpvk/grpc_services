#include <iostream>
#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"
#include <thread>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using STRM::MultiReq;
using STRM::Reply;
using STRM::Request;

class ApiService {
 public:
  ApiService(std::shared_ptr<Channel> chan)
      : stub_(STRM::MultiReq::NewStub(chan)) {}
  bool readLong(std::string&& message, int pause, int count) {
    // Структура с аргументами запроса
    Request rq;
    // Единичный ответ от сервера
    Reply rp;
    // Контекст запроса
    ClientContext ctx;

    // Начало запроса
    auto rwr = stub_->action(&ctx);

    std::thread rth([&] {
      while (rwr->Read(&rp)) {
        std::cout << " - " << rp.message() << std::endl;
      }
    });
    std::thread wth([&] {
      for (int i = 0; i < 100; i++) {
        rq.set_message("Client");
        rwr->Write(rq);
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });

    rth.join();
    wth.join();
    // Чтение результатов по 1

    // Завершение запроса
    auto stat = rwr->Finish();
    if (!stat.ok()) {
      std::cout << "Error" << std::endl;
      return false;
    }
    std::cout << "Work is done" << std::endl;
    return true;
  }

 private:
  std::unique_ptr<MultiReq::Stub> stub_;
};

int main(int argc, char* argv[]) {
  ApiService service(
      grpc::CreateChannel("localhost:8090",
                          grpc::InsecureChannelCredentials()));  // Канал связи
  service.readLong("Client", 1, 10);
  return 0;
}