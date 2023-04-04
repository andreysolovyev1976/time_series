From here

[https://interactivebrokers.github.io/tws-api/historical_bars.html#hd_what_to_show]("Link to Interactive Brokers website")

All different kinds of historical data are returned in the form of candlesticks and as such the values return represent the state of the market during the period covered by the candlestick.

| Type | Open |	High |	Low | Close | Volume |
| --- | --- | --- | --- | --- | --- |
| TRADES | First  traded | price | Highest traded price | Lowest traded price | Last traded price | Total traded volume |
| MIDPOINT | Starting  midpoint price | Highest midpoint price | Lowest midpoint price | Last midpoint price | N/A |
| BID | Starting  bid price | Highest bid price | Lowest bid price | Last bid price | N/A |
| ASK | Starting  ask price | Highest ask price | Lowest ask price | Last ask price | N/A |
| BID_ASK | Time  average bid | Max Ask | Min Bid | Time average ask | N/A |
| ADJUSTED_LAST | Dividend - adjusted first traded price |	Dividend-adjusted high trade | Dividend-adjusted low trade | Dividend-adjusted last trade | Total traded volume |
| HISTORICAL_VOLATILITY | Starting  volatility | Highest volatility | Lowest volatility | Last volatility | N/A |
| OPTION_IMPLIED_VOLATILITY | Starting  implied volatility | Highest implied volatility | Lowest implied volatility | Last implied volatility | N/A |
| FEE_RATE | Starting  fee rate | Highest fee rate | Lowest fee rate | Last fee rate | N/A |
| YIELD_BID | Starting  bid yield | Highest bid yield |	Lowest bid yield | Last bid yield | N/A |
| YIELD_ASK | Starting  ask yield | Highest ask yield |	Lowest ask yield | Last ask yield | N/A |
| YIELD_BID_ASK | Time  average bid yield |	Highest ask yield | Lowest bid yield | Time average ask yield | N/A |
| YIELD_LAST | Starting  last yield | Highest last yield |	Lowest last yield | Last last yield | N/A |
| SCHEDULE | N/A | N/A | N/A | N/A | N/A |
| AGGTRADES | First  traded price | Highest traded price |	Lowest traded price | Last traded price | Total traded volume |

