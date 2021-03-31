#include <iostream>
#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"

using BSC::Processor;
using BSC::Reply;
using BSC::Request;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class ApiService {
 public:
  ApiService(std::shared_ptr<Channel> chan)
      : stub_(BSC::Processor::NewStub(chan)) {}
  bool read(std::string&& message) {
    // Структура с аргументами запроса
    Request rq;
    // Единичный ответ от сервера
    Reply rp;
    // Контекст запроса
    ClientContext ctx;

    rq.set_message(message);

    // Начало запроса
    auto stat = stub_->action(&ctx, rq, &rp);

    if (!stat.ok()) {
      std::cout << "Error" << std::endl;
      return false;
    }
    std::cout << "Work is done " << rp.message() << std::endl;
    return true;
  }

 private:
  std::unique_ptr<Processor::Stub> stub_;
};

int main(int argc, char* argv[]) {
  ApiService service(
      grpc::CreateChannel("localhost:8090",
                          grpc::InsecureChannelCredentials()));  // Канал связи
  service.read("Client");
  return 0;
}