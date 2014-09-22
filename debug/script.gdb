define script
    b Emulator::fetch_decode_execute()
    run tests/SYZYGY
    tab
    continue
    continue
    display (void *)instruction[1]
    display (void *)instruction[0]
end
