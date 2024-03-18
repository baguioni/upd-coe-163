import cProfile

def find_min_combi(num_dials: int, start: int, end: int) -> int:
    steps = 0

    for i in range(num_dials):
        digit_start = (start // (10 ** i)) % 10
        digit_end = (end // (10 ** i)) % 10

        left_step =  digit_end - digit_start
        # instead of using abs, we can use the below line to get the absolute value
        left_step = left_step if left_step >= 0 else left_step*-1

        right_step = digit_start + 10 - digit_end
        right_step = right_step if right_step >= 0 else right_step*-1

        steps += left_step if left_step < right_step else right_step

    return steps

cProfile.run('''
for i in range(100000):
    find_min_combi(4, 1234, 654321)
''')
