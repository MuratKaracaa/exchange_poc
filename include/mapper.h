#include <optional>
#include "order.h"
#include "NewOrderSingle.h"

class Mapper {
    public:
        static std::optional<Order> fromFixToOrder(const FIX42::NewOrderSingle& fixOrder);
    };
    