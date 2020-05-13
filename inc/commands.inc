      case 0x00: {
        arg = *reinterpret_cast<double*>(mem + i + 1);
        i += sizeof(int64_t);
        Push(arg);
        break;
      }
      case 0x01: {
        reg = *reinterpret_cast<uint8_t*>(mem + i + 1);
        i += sizeof(uint8_t);
        Pop(reg);
        break;
      }

      case 0x10: {
        Add();
        break;
      }
      case 0x11: {
        Sub();
        break;
      }
      case 0x12: {
        Mul();
        break;
      }
      case 0x13: {
        Div();
        break;
      }
      case 0x14: {
        Mod();
        break;
      }
      case 0x15: {
        Sqrt();
        break;
      }

      // goto
      case 0x20: {
        Goto(i);
        break;
      }

      // in
      case 0x30: {
        std::cin >> arg;
        Push(arg);
        break;
      }
      // out
      case 0x31: {
        Out();        
        break;
      }

      case 0x40: {
        End();
        return;
      }

      // Conditionals:
      // goto==
      case 0x50: {
        const size_t size = stack.GetCurSize();
        if (abs(stack.GetElement(size - 1) - stack.GetElement(size - 2)) < EPS) {
          Goto(i);
        } else {
          i += sizeof(size_t);
        }
        break;
      }
      // goto>=
      case 0x51: {
        const size_t size = stack.GetCurSize();
        const double first = stack.GetElement(size - 1);
        const double second = stack.GetElement(size - 2);
        if (first > second ||
            abs(first - second) < EPS) {
          Goto(i);
        } else {
          i += sizeof(size_t);
        }
        break;
      }
      // goto<=
      case 0x52: {
        const size_t size = stack.GetCurSize();
        const double first = stack.GetElement(size - 1);
        const double second = stack.GetElement(size - 2);
        if (first < second ||
            abs(first - second) < EPS) {
          Goto(i);
        } else {
          i += sizeof(size_t);
        }
        break;
      }
      // goto>
      case 0x53: {
        const size_t size = stack.GetCurSize();
        if (stack.GetElement(size - 1) > stack.GetElement(size - 2)) {
          Goto(i);
        } else {
          i += sizeof(size_t);
        }
        break;
      }
      // goto<
      case 0x54: {
        const size_t size = stack.GetCurSize();
        if (stack.GetElement(size - 1) < stack.GetElement(size - 2)) {
          Goto(i);
        } else {
          i += sizeof(size_t);
        }
        break;
      }

      // Store to stack
      case 0x60: {
        reg = *reinterpret_cast<uint8_t*>(mem + i + 1);
        i += sizeof(uint8_t);
        stack.Push(this->reg[reg]);
