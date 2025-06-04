from sniper_config import parse_config, get_config

# Some test cases to show how to set and read array config variables.
def test_array_config_variables():
    import StringIO

    test_config = """
[scheduler/open/dvfs]
min_frequency[] = 3.6,3.6,2.7,2.7
min_frequency = 1  # order of default value does not matter
    """

    config_file = StringIO.StringIO(test_config)
    #cfg = parse_config(file('../config/base.cfg').read())
    cfg = parse_config(config_file.read())

    # print some config values
    #import pdb; pdb.set_trace()

    # Will print the default value
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency')

    # Will return the third value
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency', index=2)

    # Only four values, will return the default value
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency', index=10)

    # This will not work when a default value is specified, because the
    # default scalar value is returned 
    print "[{}]".format(get_config(cfg, 'scheduler/open/dvfs/min_frequency')[2])

    # So when you specify an array key with [] you should always specify a
    # default value as well. This is not explictly mentioned in the sniper
    # manual.


    # Now test without default value, this is not the recommended way
    # to specify an array value.
    test_config = """
[scheduler/open/dvfs]
min_frequency[] = 3.6,3.6,2.7,2.7
    """

    config_file = StringIO.StringIO(test_config)
    #cfg = parse_config(file('../config/base.cfg').read())
    cfg = parse_config(config_file.read())

    # Without a default value, get_config returns the list that we need
    # index ourselves.
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency')[0]
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency')[3]

    # Which can go wrong ofcourse:
    try:
        print get_config(cfg, 'scheduler/open/dvfs/min_frequency')[10]
    except IndexError:
        print('Got expected out-of-bounds error')

    # Using the index keyword has no effect, we still just get the list.
    print get_config(cfg, 'scheduler/open/dvfs/min_frequency', index=1)


# We don't have pytest for python2, so just test this way.
if __name__ == '__main__':

    test_array_config_variables()
