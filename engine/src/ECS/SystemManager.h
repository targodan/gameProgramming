#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

namespace engine {
    namespace ECS {
        class SystemManager {
        public:
            SystemManager();
            SystemManager(const SystemManager& orig);
            virtual ~SystemManager();
        private:

        };
    }
}

#endif /* SYSTEMMANAGER_H */

