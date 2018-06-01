#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
using namespace eosio;

class allowancesample : public eosio::contract
{
  public:
    using contract::contract;

    /// @abi action
    void allow(account_name sender, account_name allowed, asset quantity)
    {
        require_auth(sender);

        allowancetable allowances(_self, sender);

        auto iter = allowances.find(allowed);
        if (iter == allowances.end())
            allowances.emplace(sender, [&](auto &row) {
                row.allowed = allowed;
                row.quantity = quantity;
            });
        else
            allowances.modify(iter, 0, [&](auto &row) {
                row.quantity = quantity;
            });
    }

  private:
    /// @abi table
    struct allowance
    {
        account_name allowed;
        asset quantity;
        auto primary_key() const { return allowed; }

        EOSLIB_SERIALIZE(allowance, (allowed)(quantity))
    };

    typedef multi_index<N(allowance), allowance> allowancetable;
};

EOSIO_ABI(allowancesample, (allow))
