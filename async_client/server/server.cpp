#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"
#include <thread>
using namespace grpc;

//Реализация сервиса унаследована от базового класса сервиса
class ApiImpl : public BSC::Processor::Service {
  // Метод, который описан в proto файле должен быть переопределен
  // Метод вызывается по факту запроса от клента
  virtual ::grpc::Status action(::grpc::ServerContext* context,
                                const ::BSC::Request* request,
                                ::BSC::Reply* response) override {
    response->set_message(request->message());
    return grpc::Status::OK;
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