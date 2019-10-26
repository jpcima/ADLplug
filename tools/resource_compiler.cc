//          Copyright Jean Pierre Cimalando 2018-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <yaml-cpp/yaml.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

struct FileMetadata {
    std::string id;
    std::string source;
};

struct Metadata {
    std::string ns;
    std::string htext;
    std::vector<FileMetadata> files;
};

class Bad_Document : public std::runtime_error {
public:
    explicit Bad_Document(const std::string &msg = "invalid document")
        : runtime_error(msg) {}
};

Metadata read_from_document(const std::vector<YAML::Node> &doc)
{
    Metadata md;

    if (doc.size() != 1)
        throw Bad_Document();

    const YAML::Node &root = doc[0];
    md.ns = root["namespace"].as<std::string>();
    md.htext = root["header-text"].as<std::string>();

    const YAML::Node &files = root["files"];
    for (size_t i = 0, n = files.size(); i < n; ++i) {
        FileMetadata fmd;
        const YAML::Node &file = files[i];
        fmd.id = file["id"].as<std::string>();
        fmd.source = file["source"].as<std::string>();
        md.files.emplace_back(std::move(fmd));
    }

    return md;
}

void generate_resources(
    const Metadata &md, const std::string &dir, const std::string &base,
    std::ostream &hh, std::ostream &cc, bool do_hh, bool do_cc)
{
    if (do_hh) {
        if (!md.htext.empty())
            hh << md.htext << "\n";

        hh <<
            "#pragma once\n"
            "#include <cstddef>\n";

        hh << "\n";

        hh <<
            "namespace " << md.ns << " {\n"
            "    struct Data {\n"
            "        const char *data;\n"
            "        std::size_t size;\n"
            "    };\n"
            "}\n";

        hh << "\n";

        hh <<
            "#if !defined(RESOURCE)\n"
            "#define RESOURCE(ns, x) namespace ns { extern ns::Data x; }\n"
            "#endif\n";
    }

    if (do_cc) {
        if (!md.htext.empty())
            cc << md.htext << "\n";

        cc << "#include \"" << base << ".h\"\n";

        cc << "\n";

        cc << "namespace " << md.ns << " {\n";
        for (const FileMetadata &fmd : md.files)
            cc << "   extern Data " << fmd.id << ";\n";
        cc << "}\n";

        cc << "\n";

        cc << "namespace " << md.ns << " {\n";
        for (const FileMetadata &fmd : md.files) {
            cc << "    const signed char rdata_" << fmd.id << "[] = { ";
            std::ifstream in;
            in.exceptions(std::ios::failbit|std::ios::badbit);
            in.open(dir + '/' + fmd.source, std::ios::binary);
            in.exceptions(std::ios::badbit);
            char buf[8192];
            const char *sep = "";
            while (in) {
                in.read(buf, sizeof(buf));
                size_t n = (size_t)in.gcount();
                for (size_t i = 0; i < n; ++i) {
                    cc << sep << (int)buf[i];
                    sep = ", ";
                }
            }

            cc << " };\n";
        }
        cc << "}\n";

        cc << "\n";

        for (const FileMetadata &fmd : md.files) {
            cc << md.ns << "::Data " << md.ns << "::" << fmd.id
               << " = { (const char *)" << md.ns << "::rdata_" << fmd.id
               << ", sizeof(" << md.ns << "::rdata_" << fmd.id << ") };\n";
        }
    }
}

bool is_path_separator(char c)
{
    switch (c) {
    case '/': return true;
#if defined(_WIN32)
    case '\\': return true;
#endif
    default: return false;
    }
}

void print_usage()
{
    std::cerr << "Usage: resource_compiler [options] <resources.yml> [destination-directory]\n"
        "  -h  Generate a header file\n";
}

int main(int argc, char *argv[])
{
    std::ostream *hh = &std::cout;
    std::ostream *cc = &std::cout;

    if (argc < 2) {
        print_usage();
        return 0;
    }

    bool do_hh = false;
    bool do_cc = true;

    for (int c; (c = getopt(argc, argv, "h")) != -1;) {
        switch (c) {
        case 'h':
            do_hh = true;
            break;
        default:
            print_usage();
            return 1;
        }
    }

    int optc = argc - optind;
    if (optc != 1 && optc != 2)
        return 1;

    std::string path = argv[optind];
    std::string dir = path;
    while (!dir.empty() && !is_path_separator(dir.back()))
        dir.pop_back();

    std::ofstream hh_file;
    std::ofstream cc_file;
    hh_file.exceptions(std::ios::failbit|std::ios::badbit);
    cc_file.exceptions(std::ios::failbit|std::ios::badbit);

    std::string base = path.substr(dir.size());
    size_t pos = base.rfind('.');
    if (pos != base.npos)
        base = base.substr(0, pos);

    if (optc == 2) {
        std::string dstdir = argv[optind + 1];
        mkdir(dstdir.c_str(), 0755);
        std::string dstfile = dstdir + '/' + base;
        if (do_hh) {
            hh_file.open(dstfile + ".h");
            hh = &hh_file;
        }
        if (do_cc) {
            cc_file.open(dstfile + ".cc");
            cc = &cc_file;
        }
    }

    while (!dir.empty() && is_path_separator(dir.back()))
        dir.pop_back();

    Metadata md = read_from_document(YAML::LoadAllFromFile(path));
    generate_resources(md, dir, base, *hh, *cc, do_hh, do_cc);

    if (do_hh)
        hh->flush();
    if (do_cc)
        cc->flush();

    return 0;
}
