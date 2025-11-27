import { NotificationsDropdown } from '@/components/NotificationsDropdown';
import { Send } from 'lucide-react';
import { Link } from 'react-router-dom';
import { Button } from '@/components/ui/button';

export const Navigation = () => {
  return (
    <nav className="fixed top-0 left-0 right-0 bg-background border-b border-border z-50">
      <div className="max-w-2xl mx-auto flex items-center justify-between h-14 px-4">
        <Link to="/" className="text-xl font-semibold text-foreground">
          Instaclone
        </Link>
        
        <div className="flex items-center gap-2">
          <NotificationsDropdown />
          <Link to="/messages">
            <Button variant="ghost" size="icon">
              <Send className="h-5 w-5" />
            </Button>
          </Link>
        </div>
      </div>
    </nav>
  );
};
