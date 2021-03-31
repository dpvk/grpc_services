#include <iostream>
#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"

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

    rq.set_message("Client");
    rq.set_pause(1);
    rq.set_num_answers(10);

    // Начало запроса
    auto reader = stub_->action(&ctx, rq);

    // Чтение результатов по 1
    while (reader->Read(&rp)) {
      std::cout << " - " << rp.message() << std::endl;
    }

    // Завершение запроса
    auto stat = reader->Finish();
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