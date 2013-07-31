#include "sparrow/master.h"
#include "sparrow/worker.h"
#include "sparrow/util/common.h"

using namespace sparrow;

class PutKernel: public Kernel {
public:
  void run() {
    Table* t = get_table(0);
    if (shard_id() == 0) {
      t->put("Hello.", "World.");
    }
  }
};

class GetKernel: public Kernel {
public:
  void run() {
    Table* t = get_table(0);
    LOG(INFO)<< shard_id() << " : " << t->get("Hello.");
  }
};

REGISTER_KERNEL(PutKernel);
REGISTER_KERNEL(GetKernel);

int main(int argc, char** argv) {
  sparrow::Init(argc, argv);

  if (!StartWorker()) {
    Master m;
    LOG(INFO)<< "here.";
    Table* t = m.create_table();
    m.map_shards(t, "PutKernel");
    m.map_shards(t, "GetKernel");
  }
}
