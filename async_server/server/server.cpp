#include <grpc++/grpc++.h>
#include "api.pb.h"
#include "api.grpc.pb.h"
#include <thread>
using namespace grpc;

int main(int argc, char* argv[]) {
  // Создание сервиса
  BSC::Processor::AsyncService service;

  ServerBuilder builder;

  // Добавление endpoint
  builder.AddListeningPort("0.0.0.0:8090", grpc::InsecureServerCredentials());

  // Добавление сервиса
  builder.RegisterService(&service);

  // Получение рабочего сервиса
  std::cout << "Server started " << std::endl;
  auto cq_ = builder.AddCompletionQueue();
  // Запуск и ожидание событий
  std::unique_ptr<Server> server(builder.BuildAndStart());

  ServerContext context;
  BSC::Request request;
  ServerAsyncResponseWriter<BSC::Reply> responder(&context);
  service.Requestaction(&context, &request, &responder, cq_.get(), cq_.get(),
                        (void*)1);

  void* tag;  // uniquely identifies a request.
  bool ok;
  while (cq_->Next(&tag, &ok)) {
    if (tag == (void*)1) {
      std::cout << "Request processing" << std::endl;
      BSC::Reply reply;
      Status status;
      reply.set_message(request.message());
      responder.Finish(reply, status, (void*)2);
    }
    if (tag == (void*)2) {
      std::cout << "All work is done. Finalize response" << std::endl;
    }
  }
  return 0;
}