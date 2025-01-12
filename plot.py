"""Make some plots."""
import matplotlib.pyplot as plt

TEST_PATH="test_dump.txt"

def read_value_dump(path):
    net_worths = []
    with open(path) as f:
        for l in f.readlines():
            values = [float(v) for v in l.strip().split()]
            net_worths.append(values)
    return net_worths

if __name__ == "__main__":
    net_worths = read_value_dump(TEST_PATH)
    for n in net_worths:
        plt.plot(n)
    plt.ylabel("SEK")
    plt.xlabel("Month")
    plt.show()
