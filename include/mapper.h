#include <optional>
#include "order.h"
#include "NewOrderSingle.h"

class Mapper
{
public:
    static std::optional<Order> from_fix_to_order(const FIX42::NewOrderSingle &fix_order, const std::string session_id);
};
