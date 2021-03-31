#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"
#include <thread>
using namespace grpc;

//Реализация сервиса унаследована от базового класса сервиса
class ApiImpl : public STRM::MultiReq::Service {
  // Метод, который описан в proto файле должен быть переопределен
  // Метод вызывается по факту запроса от клента
  virtual ::grpc::Status action(
      ::grpc::ServerContext* context,
      const ::STRM::Request* request,
      ::grpc::ServerWriter< ::STRM::Reply>* writer) override {
    for (int i = 0; i < request->num_answers(); i++) {
      STRM::Reply r;
      r.set_message(request->message());
      std::this_thread::sleep_for(std::chrono::seconds(request->pause()));
      writer->Write(r);
    }
    return Status::OK;
  }
};

int main(int argc, char* argv[]) {
  // Создание сервиса
  ApiImpl service;

  ServerBuilder builder;

  // Добавление endpoint
  builder.AddListeningPort("0.0.0.0:8090", grpc::InsecureServerCredentials());

  // Добавление сервиса
  builder.RegisterService(&service);

  // Получение рабочего сервиса
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server started " << std::endl;

  // Запуск и ожидание событий
  server->Wait();
  return 0;
}