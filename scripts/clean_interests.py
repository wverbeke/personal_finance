from datetime import datetime 
from collections import defaultdict
    
def rates_and_dates(path):
    dates = []
    rates = []
    with open(path) as f:
        for l in f.readlines():
            date, rate = l.strip().split()
            date = datetime.strptime(date, "%Y-%m-%d")
            dates.append(date)

            # N/A is equivalent to last value 
            rate = rate.replace(",", ".")
            rate = rates[-1] if rate == "N/A" else rate
            rates.append(float(rate))
    return dates, rates

    
def average_by_month(dates, rates):
    grouped_rates = defaultdict(list)
    for r, d in zip(rates, dates):
        grouped_rates[(d.year, d.month)].append(r)
    avg_rates = {f"{d[0]}-{d[1]}": (sum(rates)/len(rates)) for d, rates in grouped_rates.items()}
    return avg_rates

def write_cleaned(path, avg_rates):
    with open(path, "w") as f:
        for d, r in avg_rates.items():
            f.write(f"{d} {str(r)}\n")


INTEREST_RATE_FILE = "data/riksbanken_ranta.txt"
OUT_FILE = "data/riksbanken_ranta_cleaned.txt"
if __name__ == "__main__":
    dates, rates = rates_and_dates(INTEREST_RATE_FILE)
    avg_rates = average_by_month(dates, rates)
    write_cleaned(OUT_FILE, avg_rates)
