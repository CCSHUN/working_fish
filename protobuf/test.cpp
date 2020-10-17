/*
  利用protobuf反射机制创建对象
*/
#include "query.pb.h"
#include <google/protobuf/descriptor.h>
#include <memory>
#include <iostream>
#include <functional>

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
typedef std::function<void (const std::shared_ptr<google::protobuf::Message>& message)> MessageCb;
typedef std::map<const google::protobuf::Descriptor*, MessageCb> CallbackMap;

void OnQuery(const std::shared_ptr<google::protobuf::Message>& message) {
  //向下转换
  auto query = std::static_pointer_cast<first_package::Query>(message);
  //反序列化
  std::cout<<"OnQuery:"<<std::endl;
  std::cout<<"id="<<query->id()<<std::endl;
  std::cout<<"questioner="<<query->questioner()<<std::endl;
  //std::cout<<"question="<<query->question()<<std::endl;
  for (int i = 0; i < query->question_size(); i++) {
    std::cout<<"question="<<query->question(i)<<std::endl;
  }
}

//利用protobuf反射机制 根据类型动态创建对象
google::protobuf::Message* createMessage(const std::string& typeName)
{
  google::protobuf::Message* message = NULL;
  const google::protobuf::Descriptor* descriptor =
    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
  if (descriptor)
  {
    const google::protobuf::Message* prototype =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (prototype)
    {
      message = prototype->New();
    }
  }
  return message;
}

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  //input
  first_package::Query query;
  query.set_id(1);
  query.set_questioner("chen");
  query.add_question("who are you?");
  std::string input = query.SerializeAsString(); //作为string序列化
  std::cout<<"input:"<<std::endl;
  std::cout<<input<<std::endl;
  std::cout<<query.GetTypeName()<<std::endl;

  //绑定反射机制
  CallbackMap cbs_map;
  cbs_map[first_package::Query::descriptor()] = std::bind(&OnQuery, std::placeholders::_1);
  //output
  MessagePtr message;
  message.reset(createMessage(query.GetTypeName())); //根据类型名反射
  if (message) {
    bool succ = message->ParseFromString(input);
    if (succ) {
      auto it = cbs_map.find(message->GetDescriptor());
      if (it != cbs_map.end()) {
        it->second(message);
      }
    }
  }

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}