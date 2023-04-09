//
// Created by Andrey Solovyev on 29/03/2023.
//

#include <gtest/gtest.h>
#include "tslib.h"
#include "finlib.h"

using sq_t = time_series::financial::SingleQuote<int>;
using ohlcv_t = time_series::financial::OHLCV<int>;
using bid_ask_t = time_series::financial::BidAsk<int>;
using duration_t = culib::time::Minutes;
using ts_t = culib::time::Timestamp<duration_t>;
using elem_sq_t = time_series::Element<duration_t, sq_t>;
using elem_bid_ask_t = time_series::Element<duration_t, bid_ask_t>;
using elem_ohlcv_t = time_series::Element<duration_t, ohlcv_t>;

TEST(ElementStructuredBindings, Read_SingleValue) {
	ts_t ts;
	elem_sq_t elem (ts, 1);
	auto const [ts_elem, sq_elem] = elem;
	ASSERT_EQ(ts, ts_elem);
	ASSERT_EQ(sq_elem, 1);
}
TEST(ElementStructuredBindings, Read_BidAsk) {
	ts_t ts;
	elem_bid_ask_t elem (ts, {1, 2, 3, 4, 5});
	auto const [ts_elem, bid_ask_elem] = elem;
	ASSERT_EQ(ts, ts_elem);
	auto const [bid, ask, middle, price, volume] = bid_ask_elem;

	ASSERT_EQ(bid, bid_ask_elem.bid);
	ASSERT_EQ(ask, bid_ask_elem.ask);
	ASSERT_EQ(middle, bid_ask_elem.middle);
	ASSERT_EQ(price, bid_ask_elem.price);
	ASSERT_EQ(volume, bid_ask_elem.volume);

	ASSERT_EQ(bid, 1);
	ASSERT_EQ(ask, 2);
	ASSERT_EQ(middle, 3);
	ASSERT_EQ(price, 4);
	ASSERT_EQ(volume, 5);
}
TEST(ElementStructuredBindings, Read_OHLCV) {
	ts_t ts;
	elem_ohlcv_t elem (ts, {1, 2, 3, 4, 5});
	auto const [ts_elem, ohlcv_elem] = elem;
	ASSERT_EQ(ts, ts_elem);
	auto const [open, high, low, close, volume] = ohlcv_elem;
	ASSERT_EQ(open, ohlcv_elem.open);
	ASSERT_EQ(high, ohlcv_elem.high);
	ASSERT_EQ(low, ohlcv_elem.low);
	ASSERT_EQ(close, ohlcv_elem.close);
	ASSERT_EQ(volume, ohlcv_elem.volume);

	ASSERT_EQ(open, 1);
	ASSERT_EQ(high, 2);
	ASSERT_EQ(low, 3);
	ASSERT_EQ(close, 4);
	ASSERT_EQ(volume, 5);
}
TEST(ElementStructuredBindings, Assign_SingleValue) {
	ts_t ts;
	elem_sq_t elem (ts, 1);
	auto & [ts_elem, sq_elem] = elem;

	sq_elem += 1;

	ASSERT_EQ(sq_elem, 2);
	ASSERT_EQ(elem.value, 2);
	ASSERT_EQ(ts, ts_elem);
}
TEST(ElementStructuredBindings, Assign_BidAsk) {
	ts_t ts;
	elem_bid_ask_t elem (ts, {1, 2, 3, 4, 5});
	auto & [ts_elem, bid_ask_elem] = elem;
	auto & [bid, ask, middle, price, volume] = bid_ask_elem;

	bid += 1;
	ask += 1;
	middle += 1;
	price += 1;
	volume += 1;

	ASSERT_EQ(bid, 2);
	ASSERT_EQ(ask, 3);
	ASSERT_EQ(middle, 4);
	ASSERT_EQ(price, 5);
	ASSERT_EQ(volume, 6);

	ASSERT_EQ(elem.value.bid, 2);
	ASSERT_EQ(elem.value.ask, 3);
	ASSERT_EQ(elem.value.middle, 4);
	ASSERT_EQ(elem.value.price, 5);
	ASSERT_EQ(elem.value.volume, 6);

	ASSERT_EQ(ts, ts_elem);
}
TEST(ElementStructuredBindings, Assign_OHLCV) {
	ts_t ts;
	elem_ohlcv_t elem (ts, {1, 2, 3, 4, 5});
	auto & [ts_elem, ohlcv_elem] = elem;
	auto & [open, high, low, close, volume] = ohlcv_elem;

	open += 1;
	high += 1;
	low += 1;
	close += 1;
	volume += 1;

	ASSERT_EQ(open, 2);
	ASSERT_EQ(high, 3);
	ASSERT_EQ(low, 4);
	ASSERT_EQ(close, 5);
	ASSERT_EQ(volume, 6);

	ASSERT_EQ(elem.value.open, 2);
	ASSERT_EQ(elem.value.high, 3);
	ASSERT_EQ(elem.value.low, 4);
	ASSERT_EQ(elem.value.close, 5);
	ASSERT_EQ(elem.value.volume, 6);

	ASSERT_EQ(ts, ts_elem);
}
TEST(ElementStructuredBindings, LValueRef) {
	ts_t ts;
	elem_ohlcv_t elem ({1, 2, 3, 4, 5});
	auto & [ts_elem, ohlcv_elem] = elem;
	auto & [open, high, low, close, volume] = ohlcv_elem;

	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(open)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(high)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(low)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(close)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(volume)>);
}
TEST(ElementStructuredBindings, LValueRefConst) {
	ts_t ts;
	elem_ohlcv_t elem ({1, 2, 3, 4, 5});
	auto const& [ts_elem, ohlcv_elem] = elem;
	auto const& [open, high, low, close, volume] = ohlcv_elem;

	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(open)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(high)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(low)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(close)>);
	ASSERT_TRUE(std::is_lvalue_reference_v<decltype(volume)>);

	ASSERT_TRUE(std::is_const_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_const_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_const_v<decltype(open)>);
	ASSERT_TRUE(std::is_const_v<decltype(high)>);
	ASSERT_TRUE(std::is_const_v<decltype(low)>);
	ASSERT_TRUE(std::is_const_v<decltype(close)>);
	ASSERT_TRUE(std::is_const_v<decltype(volume)>);
}
TEST(ElementStructuredBindings, RValueRef) {
	ts_t ts;
	elem_ohlcv_t elem ({1, 2, 3, 4, 5});
	auto && [ts_elem, ohlcv_elem] = elem;
	auto && [open, high, low, close, volume] = ohlcv_elem;

	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(open)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(high)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(low)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(close)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(volume)>);
}
TEST(ElementStructuredBindings, RValueRefConst) {

	auto const&& [ts_elem, ohlcv_elem] = elem_ohlcv_t ({1, 2, 3, 4, 5});
	auto const&& [open, high, low, close, volume] = elem_ohlcv_t ({1, 2, 3, 4, 5}).value;

	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(open)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(high)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(low)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(close)>);
	ASSERT_TRUE(std::is_rvalue_reference_v<decltype(volume)>);

	ASSERT_TRUE(std::is_const_v<decltype(ts_elem)>);
	ASSERT_TRUE(std::is_const_v<decltype(ohlcv_elem)>);
	ASSERT_TRUE(std::is_const_v<decltype(open)>);
	ASSERT_TRUE(std::is_const_v<decltype(high)>);
	ASSERT_TRUE(std::is_const_v<decltype(low)>);
	ASSERT_TRUE(std::is_const_v<decltype(close)>);
	ASSERT_TRUE(std::is_const_v<decltype(volume)>);
}
