#include <filesystem>
#include <iostream>
#include <vector>

extern "C" {
#include "vendor/g719/include/cnst.h"
#include "vendor/g719/include/state.h"
#include "vendor/g719/include/proto.h"
#include "vendor/g719/basicop/typedef.h"
}

#include <fstream>
#include <span>

static Word16 read_data(std::ifstream& infile, std::span<Word16> data);
static Word16 read_bitstream(std::ifstream& infile,
                             Word16 bitstream[],
                             Word16 *num_bits,
                             Word16 *bfi);

constexpr auto bitrate = 64000;

int main() {
    CoderState coder;
    DecoderState decoder;
    Word16 input[FRAME_LENGTH];
    Word16 bitstream[MAX_BITS_PER_FRAME];
    Word16 samples;

    std::span<Word16> in { input, FRAME_LENGTH };
    std::span<Word16> out { bitstream, MAX_BITS_PER_FRAME };

    std::ifstream fin("../assets/signal.raw", std::ios::binary);
    std::ofstream fout("../out/result.raw", std::ios::binary);

    if (!fin.is_open() || !fout.is_open()) {
        std::cerr << "Error opening files" << std::endl;
        return -1;
    }

    auto num_bits = static_cast<Word16>(bitrate / 50);
    std::vector<Word16> bytes(num_bits / 16);

    encoder_init(&coder, num_bits);

    do {
        samples = read_data(fin, input);

        encode_frame(in.data(), num_bits, out.data(), &coder);

        UWord16   G192_HEADER[2];

        G192_HEADER[0] = G192_SYNC_GOOD_FRAME;
        G192_HEADER[1] = (UWord16) num_bits;

        for (int i = 0; i < num_bits; ++i) {
            auto current_bit = bitstream[i] == G192_BIT0 ? 0 : 1;
            bytes[i / 16] |= (current_bit << (i % 16));
        }

        fout.write(reinterpret_cast<char*>(G192_HEADER), 2 * sizeof(UWord16));
        fout.write(reinterpret_cast<char*>(bytes.data()), num_bits / 16 * sizeof(Word16));

    } while (samples == FRAME_LENGTH);

    fin.close();
    fout.close();

    fin.open("../out/result.raw");
    fout.open("../out/decoded.raw");

    if (!fin.is_open() || !fout.is_open()) {
        std::cerr << "Error opening files" << std::endl;
        return -1;
    }

    decoder_init(&decoder, num_bits);

    Word16 bfi;
    Word16 dec_out[FRAME_LENGTH];

    while (read_bitstream(fin, bitstream, &num_bits, &bfi)) {
        if (!bfi) {
            decoder.num_bits_spectrum_stationary = sub(num_bits, 3);
            decoder.num_bits_spectrum_transient  = sub(num_bits, 1);
            decoder.num_bits                     = num_bits;
        }

        decode_frame(bitstream, bfi, dec_out, &decoder);

        fout.write(reinterpret_cast<char*>(dec_out), FRAME_LENGTH * sizeof(Word16));
    }

    return 0;
}

static Word16 read_data(std::ifstream& infile, std::span<Word16> data)
{
    Word16   i;
    Word16   data16[FRAME_LENGTH];

    infile.read(reinterpret_cast<char*>(data16), data.size_bytes());
    Word16 n = infile.gcount() / sizeof(Word16);

    for (i = 0; i < n; i++)
    {
        data[i] = data16[i];
    }

    for (i = n; i < data.size(); i++)
    {
        data[i] = 0;
    }
    return n;
}

static Word16 read_bitstream(std::ifstream &infile,
                             Word16 bitstream[],
                             Word16 *num_bits,
                             Word16 *bfi)
{
    UWord16   G192_SYNC_WORD;
    Word16    result;
    Word16    bytes[FRAME_LENGTH];

    infile.read(reinterpret_cast<char*>(&G192_SYNC_WORD), sizeof(UWord16));
    if (infile.gcount() != sizeof(UWord16))
    {
        result = 0;
    }
    else {
        if((G192_SYNC_WORD != G192_SYNC_GOOD_FRAME) && (G192_SYNC_WORD != G192_SYNC_BAD_FRAME))
        {
            fprintf(stderr, "\n Invalid bitstream. Wrong G192_SYNC_WORD ");
            exit(EXIT_FAILURE);
        }
        else {
            if(G192_SYNC_WORD == G192_SYNC_BAD_FRAME) {
                *bfi = 1;
            }
            else {
                *bfi = 0;
            }

            infile.read(reinterpret_cast<char*>(num_bits), sizeof(Word16));
            if(infile.gcount() != sizeof(Word16)) {
                fprintf(stderr, "\n Premature end of file, cannot read frame length  ");
                exit(EXIT_FAILURE);
            }

            if(*num_bits > MAX_BITS_PER_FRAME) {
                fprintf(stderr, "\n Frame is too large  ");
                exit(EXIT_FAILURE);
            }

            infile.read(reinterpret_cast<char*>(bytes), *num_bits / 16 * sizeof(Word16));
            if(infile.gcount() != (UWord16)*num_bits / 16 * sizeof(Word16))
            {
                fprintf(stderr, "\n Premature end of file, cannot read frame");
                exit(EXIT_FAILURE);
            }
            else {
                for (int i = 0; i < *num_bits; i++) {
                    bitstream[i] = ((bytes[i / 16] >> (i & 15)) & 1) ? G192_BIT1 : G192_BIT0;
                }
            }
        }
        result = 1;
    }

    return(result);
}
