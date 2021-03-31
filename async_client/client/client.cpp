#include <iostream>
#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"

using BSC::Processor;
using BSC::Reply;
using BSC::Request;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CompletionQueue;
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
    auto rpc = stub_->PrepareAsyncaction(&ctx, rq, &cq_);
    rpc->StartCall();
    Status status;
    // Запуск запроса. Последний аргумент указатель на объект запроса или другой
    // уникальный идентификатор
    rpc->Finish(&rp, &status, (void*)1);
    void* got_tag;
    bool ok = false;

    // До получения ответа чтение из очереди будет заблокировано.
    // обработчик может быть вне запроса
    // ответ может быть обработан как callback
    if (!cq_.Next(&got_tag, &ok)) {
      std::cout << "CQ Next Error" << std::endl;
    }

    if (!status.ok()) {
      std::cout << "Error" << std::endl;
      return false;
    }
    std::cout << "Work is done " << rp.message() << std::endl;
    return true;
  }

 private:
  // Очередь может быть глобальной
  grpc::CompletionQueue cq_;
  std::unique_ptr<Processor::Stub> stub_;
};

int main(int argc, char* argv[]) {
  ApiService service(
      grpc::CreateChannel("localhost:8090",
                          grpc::InsecureChannelCredentials()));  // Канал связи
  service.read("Client");
  return 0;
}